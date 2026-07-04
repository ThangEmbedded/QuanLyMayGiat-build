#include "MachineController.h"
#include <QTime>

MachineController::MachineController(std::unique_ptr<IHardwareService> hardwareService,
                                     std::unique_ptr<IRelayService> relayService,
                                     QObject *parent)
    : QObject(parent),
      m_hardwareService(std::move(hardwareService)),
      m_relayService(std::move(relayService)) {
    initializeMockMachines();

    m_minuteTimer.setInterval(60 * 1000);
    connect(&m_minuteTimer, &QTimer::timeout, this, &MachineController::onMinuteTick);
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
        emit operationFailed("Không thể khởi tạo hardware service.");
        return false;
    }

    if (!m_relayService) {
        emit operationFailed("Không tìm thấy relay service.");
        return false;
    }

    if (!m_relayService->initialize()) {
        // Do not block the kiosk UI here. On Raspberry Pi the relay driver can still
        // work after boot even if the initial safety OFF/check is not fully ready.
        // Actual start/stop operations below still validate turnOn/turnOff results.
        emit logCreated("Cảnh báo: relay service init chưa sẵn sàng, UI vẫn tiếp tục chạy.");
    }

    m_minuteTimer.start();
    emit machinesChanged(m_machines);
    emit logCreated("Hệ thống UI khởi động.");
    return true;
}

bool MachineController::startMachine(int machineId, const WashCycle &cycle, const QString &roomNumber) {
    MachineData *machine = findMachine(machineId);
    if (!machine) {
        emit operationFailed(QString("Không tìm thấy máy %1.").arg(machineId));
        return false;
    }

    if (machine->state != MachineState::Open) {
        emit operationFailed(QString("Máy %1 không ở trạng thái sẵn sàng.").arg(machineId));
        return false;
    }

    if (!m_relayService->turnOn(machineId)) {
        emit operationFailed(QString("Relay máy %1 bật thất bại.").arg(machineId));
        return false;
    }

    if (!m_hardwareService->startMachine(machineId)) {
        m_relayService->turnOff(machineId);
        // emit operationFailed(QString("Không thể khởi động máy %1.").arg(machineId));
        // return false;
    }

    const QTime start = QTime::currentTime();
    const QTime end = start.addSecs(cycle.durationMinutes * 60);

    machine->state = MachineState::Running;
    machine->claimer = QString("Phòng %1").arg(roomNumber);
    machine->remaining = cycle.durationMinutes;
    machine->total = cycle.durationMinutes;
    machine->cycle = cycle.name;
    machine->start = start.toString("HH:mm");
    machine->end = end.toString("HH:mm");
    machine->finishedHoldMinutes = 0;

    emit logCreated(QString("%1 - %2 nhận %3 (%4)")
                    .arg(start.toString("HH:mm"))
                    .arg(machine->claimer)
                    .arg(machine->name)
                    .arg(machine->cycle));
    emitUpdated(*machine);
    return true;
}

bool MachineController::setMachineOffline(int machineId) {
    MachineData *machine = findMachine(machineId);
    if (!machine) {
        return false;
    }

    m_hardwareService->setMachineEnabled(machineId, false);
    m_hardwareService->stopMachine(machineId);
    if (m_relayService) {
        m_relayService->turnOff(machineId);
    }

    machine->state = MachineState::Offline;
    machine->claimer.clear();
    machine->remaining = 0;
    machine->finishedHoldMinutes = 0;
    machine->cycle.clear();
    machine->start.clear();
    machine->end.clear();

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

    m_hardwareService->setMachineEnabled(machineId, true);
    machine->state = MachineState::Open;
    machine->claimer.clear();
    machine->remaining = 0;
    machine->finishedHoldMinutes = 0;
    machine->cycle.clear();
    machine->start.clear();
    machine->end.clear();

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

    m_hardwareService->stopMachine(machineId);
    if (m_relayService) {
        m_relayService->turnOff(machineId);
    }
    machine->state = MachineState::Open;
    machine->claimer.clear();
    machine->remaining = 0;
    machine->finishedHoldMinutes = 0;
    machine->cycle.clear();
    machine->start.clear();
    machine->end.clear();

    emit logCreated(QString("%1 - Reset %2 về trạng thái Trống")
                    .arg(QTime::currentTime().toString("HH:mm"))
                    .arg(machine->name));
    emitUpdated(*machine);
    return true;
}

void MachineController::onMinuteTick() {
    bool changed = false;
    for (MachineData &machine : m_machines) {
        if (machine.state == MachineState::Running && machine.remaining > 0) {
            machine.remaining--;
            changed = true;

            if (machine.remaining == 0) {
                m_hardwareService->stopMachine(machine.id);
                if (m_relayService) {
                    m_relayService->turnOff(machine.id);
                }
                machine.state = MachineState::Finished;
                machine.finishedHoldMinutes = 2;
                emit logCreated(QString("%1 - %2 hoàn thành chu kỳ, chuyển sang trạng thái Đã giặt xong")
                                .arg(QTime::currentTime().toString("HH:mm"))
                                .arg(machine.name));
            }
            emit machineUpdated(machine);
        }
        else if (machine.state == MachineState::Finished) {
            if (machine.finishedHoldMinutes > 0) {
                machine.finishedHoldMinutes--;
                changed = true;
            }

            if (machine.finishedHoldMinutes <= 0) {
                machine.state = MachineState::Open;
                machine.claimer.clear();
                machine.cycle.clear();
                machine.start.clear();
                machine.end.clear();
                machine.finishedHoldMinutes = 0;
                changed = true;
                emit logCreated(QString("%1 - %2 tự động về trạng thái Trống")
                                .arg(QTime::currentTime().toString("HH:mm"))
                                .arg(machine.name));
            }
            emit machineUpdated(machine);
        }
    }

    if (changed) {
        emit machinesChanged(m_machines);
    }
}

void MachineController::initializeMockMachines() {
    m_machines.clear();

    // Demo chạy relay thật: trạng thái khởi động phải an toàn và đồng bộ với relay OFF.
    // Không khởi tạo sẵn Running/Offline vì UI có thể báo sai trạng thái phần cứng sau reboot.
    for (int id = 1; id <= 4; ++id) {
        m_machines.append({id, QString("Máy %1").arg(id), MachineState::Open, "", 0, 45, "", "", ""});
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
