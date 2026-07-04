#include "RealRelayService.h"
#include <QDebug>

RealRelayService::~RealRelayService() {
    if (m_initialized) {
        for (int id = 1; id <= 4; ++id) {
            turnOff(id);
        }
        m_driver.deinit();
    }
}

bool RealRelayService::initialize() {
    clearError();

    if (!m_driver.init()) {
        setError("Không khởi tạo được relay::RelayDriver từ src/drivers/relay.");
        qWarning() << "RealRelayService initialize failed:" << m_lastError;
        return false;
    }

    m_initialized = true;

    // An toàn khi khởi động: relay active HIGH, tắt toàn bộ kênh trước.
    for (int id = 1; id <= 4; ++id) {
        if (!turnOff(id)) {
            qWarning() << "Không tắt được relay khi initialize, machineId=" << id << m_lastError;
        }
    }

    qInfo() << "RealRelayService initialized using src/drivers/relay/RelayDriver.";
    return true;
}

bool RealRelayService::turnOn(int machineId) {
    clearError();
    if (!m_initialized) {
        setError("Relay service chưa initialize.");
        return false;
    }

    relay::RelayId relayId;
    if (!mapMachineToRelay(machineId, relayId)) {
        setError(QString("machineId %1 không hợp lệ.").arg(machineId));
        return false;
    }

    // Driver relay đã cấu hình active HIGH: on() => GPIO HIGH => relay bật.
    if (!m_driver.on(relayId)) {
        setError(QString("Bật relay cho máy %1 thất bại.").arg(machineId));
        return false;
    }
    return true;
}

bool RealRelayService::turnOff(int machineId) {
    clearError();
    if (!m_initialized) {
        setError("Relay service chưa initialize.");
        return false;
    }

    relay::RelayId relayId;
    if (!mapMachineToRelay(machineId, relayId)) {
        setError(QString("machineId %1 không hợp lệ.").arg(machineId));
        return false;
    }

    if (!m_driver.off(relayId)) {
        setError(QString("Tắt relay cho máy %1 thất bại.").arg(machineId));
        return false;
    }
    return true;
}

bool RealRelayService::isOn(int machineId) const {
    relay::RelayId relayId;
    if (!mapMachineToRelay(machineId, relayId)) {
        return false;
    }
    return m_driver.isOn(relayId);
}

bool RealRelayService::mapMachineToRelay(int machineId, relay::RelayId &outRelayId) {
    switch (machineId) {
    case 1:
        outRelayId = relay::RelayId::MG1;
        return true;
    case 2:
        outRelayId = relay::RelayId::MG2;
        return true;
    case 3:
        outRelayId = relay::RelayId::MG3;
        return true;
    case 4:
        outRelayId = relay::RelayId::MG4;
        return true;
    default:
        return false;
    }
}

void RealRelayService::setError(const QString &message) {
    m_lastError = message;
}

void RealRelayService::clearError() {
    m_lastError.clear();
}
