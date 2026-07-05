#include "MachineController.h"
#include <QTime>
#include <algorithm>

namespace
{
constexpr int kSensorPollIntervalMs = 1000;
constexpr int kNoCurrentTimeoutSeconds = 60;
}

MachineController::MachineController(std::unique_ptr<IHardwareService> hardwareService,
                                     std::unique_ptr<IRelayService> relayService,
                                     std::unique_ptr<ICurrentSensorService> currentSensorService,
                                     QObject *parent)
    : QObject(parent),
      m_hardwareService(std::move(hardwareService)),
      m_relayService(std::move(relayService)),
      m_currentSensorService(std::move(currentSensorService))
{
    initializeMockMachines();

    m_sensorTimer.setInterval(kSensorPollIntervalMs);
    connect(&m_sensorTimer, &QTimer::timeout, this, &MachineController::onSensorTick);
}

const QList<MachineData>& MachineController::machines() const {
    return m_machines;
}

MachineData MachineController::machineById(int machineId) const {
    const MachineData *machine = findMachine(machineId);
    return machine ? *machine : MachineData{};
}

bool MachineController::initialize() {
    if (!m_hardwareService || !m_hardwareService->initialize()) {
        emit logCreated("Cảnh báo: hardware service init chưa sẵn sàng, UI vẫn tiếp tục chạy.");
    }

    if (!m_relayService) {
        emit logCreated("Cảnh báo: relay service không tồn tại, UI vẫn tiếp tục chạy.");
    } else if (!m_relayService->initialize()) {
        emit logCreated("Cảnh báo: relay service init chưa sẵn sàng, UI vẫn tiếp tục chạy.");
    }

    if (!m_currentSensorService || !m_currentSensorService->initialize()) {
        emit logCreated("Cảnh báo: ADS1115 chưa sẵn sàng. Nếu không đo được dòng, máy sẽ tự trả về Trống sau timeout.");
    } else {
        emit logCreated("ADS1115 đã sẵn sàng để đo dòng.");
    }

    m_sensorTimer.start();
    emit machinesChanged(m_machines);
    emit logCreated("Hệ thống UI khởi động.");
    return true;
}

bool MachineController::startMachine(int machineId, const WashCycle &cycle, const QString &roomNumber) {
    Q_UNUSED(cycle)

    MachineData *machine = findMachine(machineId);
    if (!machine) {
        emit operationFailed(QString("Không tìm thấy máy %1.").arg(machineId));
        return false;
    }

    if (machine->state != MachineState::Open) {
        emit operationFailed(QString("Máy %1 không ở trạng thái sẵn sàng.").arg(machineId));
        return false;
    }

    if (!m_relayService || !m_relayService->turnOn(machineId)) {
        emit operationFailed(QString("Relay máy %1 bật thất bại.").arg(machineId));
        return false;
    }

    if (m_hardwareService && !m_hardwareService->startMachine(machineId)) {
        emit logCreated(QString("Cảnh báo: hardware start máy %1 chưa xác nhận, tiếp tục theo dõi bằng ADS1115.").arg(machineId));
    }

    const QTime start = QTime::currentTime();

    machine->state = MachineState::Running;
    machine->claimer = QString("Phòng %1").arg(roomNumber);
    machine->remaining = 0;
    machine->total = 0;
    machine->cycle = "Theo dòng điện";
    machine->start = start.toString("HH:mm");
    machine->end.clear();
    machine->finishedHoldMinutes = 0;

    m_noCurrentSeconds[machineId] = 0;
    m_elapsedSeconds[machineId] = 0;
    m_currentDetectedOnce[machineId] = false;

    emit logCreated(QString("%1 - %2 nhận %3. Bật relay và chờ ADS1115 xác nhận dòng trong %4 giây.")
                    .arg(start.toString("HH:mm"))
                    .arg(machine->claimer)
                    .arg(machine->name)
                    .arg(kNoCurrentTimeoutSeconds));
    emitUpdated(*machine);
    return true;
}

bool MachineController::setMachineOffline(int machineId) {
    MachineData *machine = findMachine(machineId);
    if (!machine) {
        return false;
    }

    if (m_hardwareService) {
        m_hardwareService->setMachineEnabled(machineId, false);
        m_hardwareService->stopMachine(machineId);
    }
    if (m_relayService) {
        m_relayService->turnOff(machineId);
    }

    machine->state = MachineState::Offline;
    machine->claimer.clear();
    machine->remaining = 0;
    machine->total = 0;
    machine->finishedHoldMinutes = 0;
    machine->cycle.clear();
    machine->start.clear();
    machine->end.clear();

    m_noCurrentSeconds.remove(machineId);
    m_elapsedSeconds.remove(machineId);
    m_currentDetectedOnce.remove(machineId);

    emit logCreated(QString("%1 - Tắt %2 (Ngoại tuyến)")
                    .arg(QTime::currentTime().toString("HH:mm"))
                    .arg(machine->name));
    emitUpdated(*machine);
    return true;
}

bool MachineController::setMachineOnline(int machineId) {
    MachineData *machine = findMachine(machineId);
    if (!machine) {
        return false;
    }

    if (m_hardwareService) {
        m_hardwareService->setMachineEnabled(machineId, true);
    }

    machine->state = MachineState::Open;
    machine->claimer.clear();
    machine->remaining = 0;
    machine->total = 0;
    machine->finishedHoldMinutes = 0;
    machine->cycle.clear();
    machine->start.clear();
    machine->end.clear();

    m_noCurrentSeconds.remove(machineId);
    m_elapsedSeconds.remove(machineId);
    m_currentDetectedOnce.remove(machineId);

    emit logCreated(QString("%1 - Bật %2")
                    .arg(QTime::currentTime().toString("HH:mm"))
                    .arg(machine->name));
    emitUpdated(*machine);
    return true;
}

bool MachineController::toggleMachineOnlineState(int machineId) {
    const MachineData *machine = findMachine(machineId);
    if (!machine) {
        return false;
    }

    if (machine->state == MachineState::Offline) {
        return setMachineOnline(machineId);
    }
    return setMachineOffline(machineId);
}

bool MachineController::resetMachine(int machineId) {
    MachineData *machine = findMachine(machineId);
    if (!machine) {
        return false;
    }

    stopAndResetMachine(*machine, "Reset thủ công");
    return true;
}

void MachineController::onSensorTick() {
    bool changed = false;

    for (MachineData &machine : m_machines) {
        if (machine.state != MachineState::Running) {
            continue;
        }

        const int machineId = machine.id;
        const bool hasCurrent = m_currentSensorService && m_currentSensorService->hasCurrent(machineId);
        const double voltage = m_currentSensorService ? m_currentSensorService->lastVoltage(machineId) : 0.0;

        m_elapsedSeconds[machineId] = m_elapsedSeconds.value(machineId, 0) + 1;
        machine.total = std::max(0, m_elapsedSeconds.value(machineId) / 60);
        machine.remaining = 0;

        if (hasCurrent) {
            if (!m_currentDetectedOnce.value(machineId, false)) {
                emit logCreated(QString("%1 - %2 đã phát hiện dòng ADS1115 (V=%3).")
                                .arg(QTime::currentTime().toString("HH:mm"))
                                .arg(machine.name)
                                .arg(voltage, 0, 'f', 3));
            }
            m_currentDetectedOnce[machineId] = true;
            m_noCurrentSeconds[machineId] = 0;
        } else {
            m_noCurrentSeconds[machineId] = m_noCurrentSeconds.value(machineId, 0) + 1;
        }

        if (m_noCurrentSeconds.value(machineId, 0) >= kNoCurrentTimeoutSeconds) {
            const QString reason = m_currentDetectedOnce.value(machineId, false)
                    ? QString("Không còn dòng tiêu thụ trong %1 giây").arg(kNoCurrentTimeoutSeconds)
                    : QString("Không phát hiện dòng sau %1 giây khởi động").arg(kNoCurrentTimeoutSeconds);
            stopAndResetMachine(machine, reason);
            changed = true;
            continue;
        }

        emit machineUpdated(machine);
        changed = true;
    }

    if (changed) {
        emit machinesChanged(m_machines);
    }
}

void MachineController::initializeMockMachines() {
    m_machines.clear();

    for (int id = 1; id <= 4; ++id) {
        m_machines.append({id, QString("Máy %1").arg(id), MachineState::Open, "", 0, 0, "", "", ""});
    }
}

MachineData* MachineController::findMachine(int machineId) {
    for (MachineData &machine : m_machines) {
        if (machine.id == machineId) {
            return &machine;
        }
    }
    return nullptr;
}

const MachineData* MachineController::findMachine(int machineId) const {
    for (const MachineData &machine : m_machines) {
        if (machine.id == machineId) {
            return &machine;
        }
    }
    return nullptr;
}

void MachineController::emitUpdated(const MachineData &machine) {
    emit machineUpdated(machine);
    emit machinesChanged(m_machines);
}

void MachineController::stopAndResetMachine(MachineData &machine, const QString &reason) {
    const int machineId = machine.id;

    if (m_hardwareService) {
        m_hardwareService->stopMachine(machineId);
    }
    if (m_relayService) {
        m_relayService->turnOff(machineId);
    }

    emit logCreated(QString("%1 - %2: %3. Tắt relay và trả về trạng thái Trống.")
                    .arg(QTime::currentTime().toString("HH:mm"))
                    .arg(machine.name)
                    .arg(reason));

    machine.state = MachineState::Open;
    machine.claimer.clear();
    machine.remaining = 0;
    machine.total = 0;
    machine.finishedHoldMinutes = 0;
    machine.cycle.clear();
    machine.start.clear();
    machine.end.clear();

    m_noCurrentSeconds.remove(machineId);
    m_elapsedSeconds.remove(machineId);
    m_currentDetectedOnce.remove(machineId);

    emit machineUpdated(machine);
}
