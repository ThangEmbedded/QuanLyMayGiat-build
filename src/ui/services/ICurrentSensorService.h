#ifndef ICURRENTSENSORSERVICE_H
#define ICURRENTSENSORSERVICE_H

class ICurrentSensorService {
public:
    virtual ~ICurrentSensorService() = default;

    virtual bool initialize() = 0;
    virtual bool hasCurrent(int machineId) = 0;
    virtual double lastVoltage(int machineId) const = 0;
};

#endif // ICURRENTSENSORSERVICE_H
