#ifndef MOCKRELAYSERVICE_H
#define MOCKRELAYSERVICE_H

#include "IRelayService.h"
#include <QSet>

class MockRelayService : public IRelayService {
public:
    bool initialize() override;
    bool turnOn(int machineId) override;
    bool turnOff(int machineId) override;
    bool isOn(int machineId) const override;

private:
    QSet<int> m_onChannels;
};

#endif // MOCKRELAYSERVICE_H
