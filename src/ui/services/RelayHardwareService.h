#ifndef RELAYHARDWARESERVICE_H
#define RELAYHARDWARESERVICE_H

#include "IRelayService.h"
#include "drivers/relay/relay.hpp"

class RelayHardwareService : public IRelayService {
public:
    RelayHardwareService() = default;
    ~RelayHardwareService() override;

    bool initialize() override;
    bool turnOn(int machineId) override;
    bool turnOff(int machineId) override;
    bool isOn(int machineId) const override;

private:
    relay::RelayDriver m_relayDriver;
    bool m_initialized{false};

    static bool isValidMachineId(int machineId);
    static relay::RelayId toRelayId(int machineId);
};

#endif // RELAYHARDWARESERVICE_H
