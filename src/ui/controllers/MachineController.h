#ifndef MACHINECONTROLLER_H
#define MACHINECONTROLLER_H

#include <QObject>
#include <QList>
#include <QTimer>
#include <QHash>
#include <memory>
#include "models/MachineData.h"
#include "models/WashCycle.h"
#include "services/IHardwareService.h"
#include "services/IRelayService.h"
#include "services/ICurrentSensorService.h"

class MachineController : public QObject {
    Q_OBJECT

public:
    explicit MachineController(std::unique_ptr<IHardwareService> hardwareService,
                               std::unique_ptr<IRelayService> relayService,
                               std::unique_ptr<ICurrentSensorService> currentSensorService,
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
    void onSensorTick();

private:
    QList<MachineData> m_machines;
    std::unique_ptr<IHardwareService> m_hardwareService;
    std::unique_ptr<IRelayService> m_relayService;
    std::unique_ptr<ICurrentSensorService> m_currentSensorService;

    QTimer m_sensorTimer;
    QHash<int, int> m_noCurrentSeconds;
    QHash<int, int> m_elapsedSeconds;
    QHash<int, bool> m_currentDetectedOnce;

    void initializeMockMachines();
    MachineData* findMachine(int machineId);
    const MachineData* findMachine(int machineId) const;
    void emitUpdated(const MachineData &machine);
    void stopAndResetMachine(MachineData &machine, const QString &reason);
};

#endif // MACHINECONTROLLER_H
