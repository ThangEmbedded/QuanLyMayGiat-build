#ifndef IRELAYSERVICE_H
#define IRELAYSERVICE_H

class IRelayService {
public:
    virtual ~IRelayService() = default;

    virtual bool initialize() = 0;
    virtual bool turnOn(int machineId) = 0;
    virtual bool turnOff(int machineId) = 0;
    virtual bool isOn(int machineId) const = 0;
};

#endif // IRELAYSERVICE_H
