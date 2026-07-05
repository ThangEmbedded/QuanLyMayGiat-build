#ifndef MOCKCURRENTSENSORSERVICE_H
#define MOCKCURRENTSENSORSERVICE_H

#include "ICurrentSensorService.h"

class MockCurrentSensorService : public ICurrentSensorService {
public:
    bool initialize() override { return true; }
    bool hasCurrent(int) override { return true; }
    double lastVoltage(int) const override { return 1.0; }
    CurrentReading readMachineCurrentState(int) override
    {
        CurrentReading reading;
        reading.ok = true;
        reading.active = true;
        reading.voltage = 1.0;
        reading.raw = 8000;
        reading.channel = 0;
        reading.debounceActiveCount = 1;
        reading.debounceRequiredCount = 1;
        return reading;
    }
};

#endif // MOCKCURRENTSENSORSERVICE_H
