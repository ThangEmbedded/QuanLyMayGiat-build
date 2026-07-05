#ifndef IHARDWARESERVICE_H
#define IHARDWARESERVICE_H

class IHardwareService {
public:
    virtual ~IHardwareService() = default;

    virtual bool initialize() = 0;
    virtual bool startMachine(int machineId) = 0;
    virtual bool stopMachine(int machineId) = 0;
    virtual bool setMachineEnabled(int machineId, bool enabled) = 0;
    virtual bool isMachineAvailable(int machineId) const = 0;
};

#endif // IHARDWARESERVICE_H
