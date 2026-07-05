#ifndef MOCKHARDWARESERVICE_H
#define MOCKHARDWARESERVICE_H

#include "IHardwareService.h"
#include <QSet>

class MockHardwareService final : public IHardwareService {
public:
    bool initialize() override;
    bool startMachine(int machineId) override;
    bool stopMachine(int machineId) override;
    bool setMachineEnabled(int machineId, bool enabled) override;
    bool isMachineAvailable(int machineId) const override;

private:
    QSet<int> m_disabledMachines;
    QSet<int> m_runningMachines;
};

#endif // MOCKHARDWARESERVICE_H
