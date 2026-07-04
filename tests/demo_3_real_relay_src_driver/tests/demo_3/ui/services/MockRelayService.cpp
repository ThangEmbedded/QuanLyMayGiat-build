#include "MockRelayService.h"

bool MockRelayService::initialize() {
    m_onChannels.clear();
    return true;
}

bool MockRelayService::turnOn(int machineId) {
    if (machineId <= 0) {
        return false;
    }
    m_onChannels.insert(machineId);
    return true;
}

bool MockRelayService::turnOff(int machineId) {
    if (machineId <= 0) {
        return false;
    }
    m_onChannels.remove(machineId);
    return true;
}

bool MockRelayService::isOn(int machineId) const {
    return m_onChannels.contains(machineId);
}
