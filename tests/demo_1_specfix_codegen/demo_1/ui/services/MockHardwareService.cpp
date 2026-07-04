#include "MockHardwareService.h"

bool MockHardwareService::initialize() {
    return true;
}

bool MockHardwareService::startMachine(int machineId) {
    if (!isMachineAvailable(machineId)) {
        return false;
    }
    m_runningMachines.insert(machineId);
    return true;
}

bool MockHardwareService::stopMachine(int machineId) {
    m_runningMachines.remove(machineId);
    return true;
}

bool MockHardwareService::setMachineEnabled(int machineId, bool enabled) {
    if (enabled) {
        m_disabledMachines.remove(machineId);
    } else {
        m_disabledMachines.insert(machineId);
        m_runningMachines.remove(machineId);
    }
    return true;
}

bool MockHardwareService::isMachineAvailable(int machineId) const {
    return machineId > 0 && !m_disabledMachines.contains(machineId);
}
