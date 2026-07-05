#ifndef MOCKCURRENTSENSORSERVICE_H
#define MOCKCURRENTSENSORSERVICE_H

#include "ICurrentSensorService.h"

class MockCurrentSensorService : public ICurrentSensorService {
public:
    bool initialize() override { return true; }
    bool hasCurrent(int) override { return true; }
    double lastVoltage(int) const override { return 1.0; }
};

#endif // MOCKCURRENTSENSORSERVICE_H
