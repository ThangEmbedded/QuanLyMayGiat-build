#ifndef ICURRENTSENSORSERVICE_H
#define ICURRENTSENSORSERVICE_H

class ICurrentSensorService {
public:
    struct CurrentReading {
        bool ok{false};
        bool active{false};
        double voltage{0.0};
        int raw{0};
        int channel{0};
        int debounceActiveCount{0};
        int debounceRequiredCount{1};
    };

    virtual ~ICurrentSensorService() = default;

    virtual bool initialize() = 0;
    virtual bool hasCurrent(int machineId) = 0;
    virtual double lastVoltage(int machineId) const = 0;
    virtual CurrentReading readMachineCurrentState(int machineId) = 0;
};

#endif // ICURRENTSENSORSERVICE_H
