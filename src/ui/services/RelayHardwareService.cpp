#include "RelayHardwareService.h"
#include <QDebug>

RelayHardwareService::~RelayHardwareService()
{
    if (m_initialized) {
        m_relayDriver.deinit();
    }
}

bool RelayHardwareService::initialize()
{
    if (m_initialized) {
        return true;
    }

    if (!m_relayDriver.init()) {
        qWarning() << "[RelayHardwareService] Relay driver init failed. UI will continue; start operation will retry.";
        return true;
    }

    m_initialized = true;

    for (int machineId = 1; machineId <= 4; ++machineId) {
        if (!m_relayDriver.off(toRelayId(machineId))) {
            qWarning() << "[RelayHardwareService] Initial relay OFF failed for machine" << machineId;
        }
    }

    return true;
}

bool RelayHardwareService::turnOn(int machineId)
{
    if (!isValidMachineId(machineId)) {
        return false;
    }

    if (!m_initialized) {
        if (!m_relayDriver.init()) {
            qWarning() << "[RelayHardwareService] Retry relay init failed on turnOn for machine" << machineId;
            return false;
        }
        m_initialized = true;
    }

    return m_relayDriver.on(toRelayId(machineId));
}

bool RelayHardwareService::turnOff(int machineId)
{
    if (!isValidMachineId(machineId)) {
        return false;
    }

    if (!m_initialized) {
        if (!m_relayDriver.init()) {
            qWarning() << "[RelayHardwareService] Retry relay init failed on turnOff for machine" << machineId;
            return false;
        }
        m_initialized = true;
    }

    return m_relayDriver.off(toRelayId(machineId));
}

bool RelayHardwareService::isOn(int machineId) const
{
    if (!isValidMachineId(machineId)) {
        return false;
    }

    return m_relayDriver.isOn(toRelayId(machineId));
}

bool RelayHardwareService::isValidMachineId(int machineId)
{
    return machineId >= 1 && machineId <= 4;
}

relay::RelayId RelayHardwareService::toRelayId(int machineId)
{
    switch (machineId) {
        case 1: return relay::RelayId::MG1;
        case 2: return relay::RelayId::MG2;
        case 3: return relay::RelayId::MG3;
        case 4: return relay::RelayId::MG4;
        default: return relay::RelayId::MG1;
    }
}
