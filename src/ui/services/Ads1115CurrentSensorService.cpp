#include "Ads1115CurrentSensorService.h"
#include "drivers/i2c/i2c_config.hpp"

#include <QDebug>
#include <algorithm>
#include <cmath>
#include <limits>

Ads1115CurrentSensorService::Ads1115CurrentSensorService(double activeSignalThreshold)
    : m_activeSignalThreshold(activeSignalThreshold)
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
        qWarning() << "[ADS1115] I2C init failed";
        return false;
    }

    if (!m_i2c.open(driver::i2c::DEFAULT_DEVICE)) {
        qWarning() << "[ADS1115] Cannot open" << driver::i2c::DEFAULT_DEVICE;
        return false;
    }

    bool anyReady = false;
    for (int index = 0; index < kMachineCount; ++index) {
        const SensorBinding &binding = kBindings[static_cast<size_t>(index)];
        auto ads = std::make_unique<ADS1115>(m_i2c, binding.address);

        if (!ads->isConnected()) {
            qWarning() << "[ADS1115] Machine" << binding.machineId
                       << "not detected at address"
                       << QString("0x%1").arg(static_cast<int>(binding.address), 2, 16, QLatin1Char('0'));
            continue;
        }

        if (!ads->init()) {
            qWarning() << "[ADS1115] Machine" << binding.machineId
                       << "init failed at address"
                       << QString("0x%1").arg(static_cast<int>(binding.address), 2, 16, QLatin1Char('0'));
            continue;
        }

        m_ads[static_cast<size_t>(index)] = std::move(ads);
        m_adsReady[static_cast<size_t>(index)] = true;
        anyReady = true;

        calibrateBaseline(index);
        qInfo() << "[ADS1115] Machine" << binding.machineId
                << "ready addr" << QString("0x%1").arg(static_cast<int>(binding.address), 2, 16, QLatin1Char('0'))
                << "AIN" << binding.channel
                << "baseline" << m_baselines[static_cast<size_t>(index)];
    }

    m_initialized = anyReady;
    return anyReady;
}

bool Ads1115CurrentSensorService::hasCurrent(int machineId)
{
    if (!isValidMachineId(machineId)) {
        return false;
    }

    if (!m_initialized && !initialize()) {
        return false;
    }

    const int index = indexForMachine(machineId);
    if (index < 0 || !m_adsReady[static_cast<size_t>(index)] || !m_ads[static_cast<size_t>(index)]) {
        return false;
    }

    double averageVoltage = 0.0;
    double signal = 0.0;
    if (!readSignal(index, averageVoltage, signal)) {
        qWarning() << "[ADS1115] Read failed for machine" << machineId;
        return false;
    }

    m_lastVoltages[static_cast<size_t>(index)] = averageVoltage;
    m_lastSignals[static_cast<size_t>(index)] = signal;

    return signal >= m_activeSignalThreshold;
}

double Ads1115CurrentSensorService::lastVoltage(int machineId) const
{
    if (!isValidMachineId(machineId)) {
        return 0.0;
    }
    return m_lastVoltages[static_cast<size_t>(indexForMachine(machineId))];
}

bool Ads1115CurrentSensorService::isValidMachineId(int machineId)
{
    return machineId >= 1 && machineId <= kMachineCount;
}

int Ads1115CurrentSensorService::indexForMachine(int machineId)
{
    return isValidMachineId(machineId) ? machineId - 1 : -1;
}

bool Ads1115CurrentSensorService::calibrateBaseline(int index)
{
    if (index < 0 || index >= kMachineCount || !m_ads[static_cast<size_t>(index)]) {
        return false;
    }

    const int channel = kBindings[static_cast<size_t>(index)].channel;
    double sum = 0.0;
    int validSamples = 0;

    for (int i = 0; i < kCalibrationSamples; ++i) {
        const double voltage = m_ads[static_cast<size_t>(index)]->readVoltage(channel);
        if (voltage >= 0.0 && std::isfinite(voltage)) {
            sum += voltage;
            ++validSamples;
        }
    }

    if (validSamples == 0) {
        return false;
    }

    m_baselines[static_cast<size_t>(index)] = sum / static_cast<double>(validSamples);
    m_lastVoltages[static_cast<size_t>(index)] = m_baselines[static_cast<size_t>(index)];
    return true;
}

bool Ads1115CurrentSensorService::readSignal(int index, double &averageVoltage, double &maxDeltaFromBaseline)
{
    if (index < 0 || index >= kMachineCount || !m_ads[static_cast<size_t>(index)]) {
        return false;
    }

    const int channel = kBindings[static_cast<size_t>(index)].channel;
    const double baseline = m_baselines[static_cast<size_t>(index)];

    double sum = 0.0;
    double maxDelta = 0.0;
    int validSamples = 0;

    for (int i = 0; i < kSamplesPerRead; ++i) {
        const double voltage = m_ads[static_cast<size_t>(index)]->readVoltage(channel);
        if (voltage < 0.0 || !std::isfinite(voltage)) {
            continue;
        }

        sum += voltage;
        maxDelta = std::max(maxDelta, std::abs(voltage - baseline));
        ++validSamples;
    }

    if (validSamples == 0) {
        return false;
    }

    averageVoltage = sum / static_cast<double>(validSamples);
    maxDeltaFromBaseline = maxDelta;
    return true;
}
