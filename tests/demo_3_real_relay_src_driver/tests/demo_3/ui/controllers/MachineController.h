#ifndef MACHINECONTROLLER_H
#define MACHINECONTROLLER_H

#include <QObject>
#include <QList>
#include <QTimer>
#include <memory>
#include "models/MachineData.h"
#include "models/WashCycle.h"
#include "services/IHardwareService.h"
#include "services/IRelayService.h"

class MachineController : public QObject {
    Q_OBJECT

public:
    explicit MachineController(std::unique_ptr<IHardwareService> hardwareService,
                               std::unique_ptr<IRelayService> relayService,
                               QObject *parent = nullptr);

    const QList<MachineData>& machines() const;
    MachineData machineById(int machineId) const;

    bool initialize();
    bool startMachine(int machineId, const WashCycle &cycle, const QString &roomNumber);
    bool setMachineOffline(int machineId);
    bool setMachineOnline(int machineId);
    bool toggleMachineOnlineState(int machineId);
    bool resetMachine(int machineId);

signals:
    void machinesChanged(const QList<MachineData> &machines);
    void machineUpdated(const MachineData &machine);
    void logCreated(const QString &logText);
    void operationFailed(const QString &message);

private slots:
    void onMinuteTick();

private:
    QList<MachineData> m_machines;
    std::unique_ptr<IHardwareService> m_hardwareService;
    std::unique_ptr<IRelayService> m_relayService;
    QTimer m_minuteTimer;

    void initializeMockMachines();
    MachineData* findMachine(int machineId);
    const MachineData* findMachine(int machineId) const;
    void emitUpdated(const MachineData &machine);
};

#endif // MACHINECONTROLLER_H
