#ifndef ADS1115CURRENTSENSORSERVICE_H
#define ADS1115CURRENTSENSORSERVICE_H

#include "ICurrentSensorService.h"
#include "drivers/i2c/i2c.hpp"
#include "drivers/ads1115/ads1115.hpp"

#include <array>
#include <memory>

class Ads1115CurrentSensorService : public ICurrentSensorService {
public:
    explicit Ads1115CurrentSensorService(double activeVoltageThreshold = 0.08);
    ~Ads1115CurrentSensorService() override;

    bool initialize() override;
    bool hasCurrent(int machineId) override;
    double lastVoltage(int machineId) const override;

private:
    LinuxI2c m_i2c;
    std::unique_ptr<ADS1115> m_ads1115;
    bool m_initialized{false};
    double m_activeVoltageThreshold{0.08};
    std::array<double, 4> m_lastVoltages{{0.0, 0.0, 0.0, 0.0}};

    static bool isValidMachineId(int machineId);
    static int channelForMachine(int machineId);
};

#endif // ADS1115CURRENTSENSORSERVICE_H
