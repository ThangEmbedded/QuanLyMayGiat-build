#ifndef REALRELAYSERVICE_H
#define REALRELAYSERVICE_H

#include "IRelayService.h"
#include "drivers/relay/relay.hpp"
#include <QString>

class RealRelayService : public IRelayService {
public:
    RealRelayService() = default;
    ~RealRelayService() override;

    bool initialize() override;
    bool turnOn(int machineId) override;
    bool turnOff(int machineId) override;
    bool isOn(int machineId) const override;

    QString lastError() const { return m_lastError; }

private:
    static bool mapMachineToRelay(int machineId, relay::RelayId &outRelayId);
    void setError(const QString &message);
    void clearError();

    relay::RelayDriver m_driver;
    bool m_initialized = false;
    QString m_lastError;
};

#endif // REALRELAYSERVICE_H
