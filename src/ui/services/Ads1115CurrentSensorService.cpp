#include "Ads1115CurrentSensorService.h"
#include "drivers/i2c/i2c_config.hpp"

#include <QDebug>
#include <algorithm>
#include <cmath>
#include <optional>

Ads1115CurrentSensorService::Ads1115CurrentSensorService()
    : m_configs(defaultConfigs())
{
}

Ads1115CurrentSensorService::Ads1115CurrentSensorService(double activeVoltageThreshold)
    : m_configs(defaultConfigs(activeVoltageThreshold))
{
}

Ads1115CurrentSensorService::Ads1115CurrentSensorService(const std::array<MachineAdcConfig, 4> &machineConfigs)
    : m_configs(machineConfigs)
{
}

Ads1115CurrentSensorService::~Ads1115CurrentSensorService()
{
    m_i2c.close();
}

bool Ads1115CurrentSensorService::initialize()
{
    if (m_initialized) {
        return true;
    }

    if (!m_i2c.init()) {
        qWarning() << "[Ads1115CurrentSensorService] I2C init failed";
        return false;
    }

    if (!m_i2c.open(driver::i2c::DEFAULT_DEVICE)) {
        qWarning() << "[Ads1115CurrentSensorService] Cannot open" << driver::i2c::DEFAULT_DEVICE;
        return false;
    }

    m_ads1115 = std::make_unique<ADS1115>(m_i2c);
    if (!m_ads1115->init()) {
        qWarning() << "[Ads1115CurrentSensorService] ADS1115 init failed";
        return false;
    }

    if (!m_ads1115->isConnected()) {
        qWarning() << "[Ads1115CurrentSensorService] ADS1115 config register read failed";
        return false;
    }

    m_initialized = true;
    return true;
}

bool Ads1115CurrentSensorService::hasCurrent(int machineId)
{
    return readMachineCurrentState(machineId).active;
}

ICurrentSensorService::CurrentReading Ads1115CurrentSensorService::readMachineCurrentState(int machineId)
{
    CurrentReading reading;

    if (!isValidMachineId(machineId)) {
        return reading;
    }

    const size_t index = static_cast<size_t>(machineId - 1);
    const MachineAdcConfig &config = m_configs[index];
    reading.channel = config.channel;
    reading.debounceRequiredCount = std::max(1, config.debounceRequiredCount);

    if (!m_initialized && !initialize()) {
        // Sensor unavailable is not equivalent to "no current". Caller can inspect ok=false.
        return reading;
    }

    std::optional<int16_t> rawValue;
    std::optional<double> voltage;
    if (config.mode == ADS1115::InputMode::Differential) {
        rawValue = m_ads1115 ? m_ads1115->readRawDifferential(config.differentialPair) : std::nullopt;
        if (rawValue.has_value()) {
            voltage = static_cast<double>(*rawValue) * (4.096 / 32768.0);
        }
    } else {
        rawValue = m_ads1115 ? m_ads1115->readRawSingleEnded(config.channel) : std::nullopt;
        if (rawValue.has_value()) {
            voltage = static_cast<double>(*rawValue) * (4.096 / 32768.0);
        }
    }

    if (!rawValue.has_value() || !voltage.has_value()) {
        qWarning() << "[Ads1115CurrentSensorService] Read failed for machine" << machineId
                   << "channel" << config.channel;
        return reading;
    }

    reading.ok = true;
    reading.raw = static_cast<int>(*rawValue);
    reading.voltage = *voltage;
    m_lastRaw[index] = reading.raw;
    m_lastVoltages[index] = reading.voltage;

    const bool sampleActive = std::abs(reading.voltage) >= config.activeVoltageThreshold;
    if (sampleActive) {
        m_activeSampleCounts[index] = std::min(reading.debounceRequiredCount, m_activeSampleCounts[index] + 1);
    } else {
        m_activeSampleCounts[index] = 0;
    }

    reading.debounceActiveCount = m_activeSampleCounts[index];
    reading.active = reading.debounceActiveCount >= reading.debounceRequiredCount;
    return reading;
}

double Ads1115CurrentSensorService::lastVoltage(int machineId) const
{
    if (!isValidMachineId(machineId)) {
        return 0.0;
    }
    return m_lastVoltages[static_cast<size_t>(machineId - 1)];
}

std::array<Ads1115CurrentSensorService::MachineAdcConfig, 4> Ads1115CurrentSensorService::defaultConfigs(double threshold)
{
    std::array<MachineAdcConfig, 4> configs{};
    for (int i = 0; i < 4; ++i) {
        configs[static_cast<size_t>(i)].mode = ADS1115::InputMode::SingleEnded;
        configs[static_cast<size_t>(i)].channel = i;
        configs[static_cast<size_t>(i)].activeVoltageThreshold = threshold;
        configs[static_cast<size_t>(i)].debounceRequiredCount = 3;
    }
    return configs;
}

bool Ads1115CurrentSensorService::isValidMachineId(int machineId)
{
    return machineId >= 1 && machineId <= 4;
}
