#include "Ads1115CurrentSensorService.h"
#include "drivers/i2c/i2c_config.hpp"

#include <QDebug>
#include <cmath>

Ads1115CurrentSensorService::Ads1115CurrentSensorService(double activeVoltageThreshold)
    : m_activeVoltageThreshold(activeVoltageThreshold)
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
        qWarning() << "[Ads1115CurrentSensorService] ADS1115 not detected";
        return false;
    }

    m_initialized = true;
    return true;
}

bool Ads1115CurrentSensorService::hasCurrent(int machineId)
{
    if (!isValidMachineId(machineId)) {
        return false;
    }

    if (!m_initialized && !initialize()) {
        return false;
    }

    const int channel = channelForMachine(machineId);
    const double voltage = m_ads1115 ? m_ads1115->readVoltage(channel) : -1.0;
    if (voltage < 0.0) {
        qWarning() << "[Ads1115CurrentSensorService] Read failed for machine" << machineId
                   << "channel" << channel;
        return false;
    }

    m_lastVoltages[static_cast<size_t>(machineId - 1)] = voltage;
    return std::abs(voltage) >= m_activeVoltageThreshold;
}

double Ads1115CurrentSensorService::lastVoltage(int machineId) const
{
    if (!isValidMachineId(machineId)) {
        return 0.0;
    }
    return m_lastVoltages[static_cast<size_t>(machineId - 1)];
}

bool Ads1115CurrentSensorService::isValidMachineId(int machineId)
{
    return machineId >= 1 && machineId <= 4;
}

int Ads1115CurrentSensorService::channelForMachine(int machineId)
{
    return machineId - 1;
}
