#ifndef ADS1115CURRENTSENSORSERVICE_H
#define ADS1115CURRENTSENSORSERVICE_H

#include "ICurrentSensorService.h"
#include "drivers/i2c/i2c.hpp"
#include "drivers/ads1115/ads1115.hpp"

#include <array>
#include <memory>

class Ads1115CurrentSensorService : public ICurrentSensorService {
public:
    struct MachineAdcConfig {
        ADS1115::InputMode mode{ADS1115::InputMode::SingleEnded};
        int channel{0};
        ADS1115::DifferentialPair differentialPair{ADS1115::DifferentialPair::Ain0Ain1};
        double activeVoltageThreshold{0.08};
        int debounceRequiredCount{3};
    };

    Ads1115CurrentSensorService();
    explicit Ads1115CurrentSensorService(double activeVoltageThreshold);
    explicit Ads1115CurrentSensorService(const std::array<MachineAdcConfig, 4> &machineConfigs);
    ~Ads1115CurrentSensorService() override;

    bool initialize() override;
    bool hasCurrent(int machineId) override;
    double lastVoltage(int machineId) const override;
    CurrentReading readMachineCurrentState(int machineId) override;

private:
    LinuxI2c m_i2c;
    std::unique_ptr<ADS1115> m_ads1115;
    bool m_initialized{false};
    std::array<MachineAdcConfig, 4> m_configs{};
    std::array<double, 4> m_lastVoltages{{0.0, 0.0, 0.0, 0.0}};
    std::array<int, 4> m_lastRaw{{0, 0, 0, 0}};
    std::array<int, 4> m_activeSampleCounts{{0, 0, 0, 0}};

    static std::array<MachineAdcConfig, 4> defaultConfigs(double threshold = 0.08);
    static bool isValidMachineId(int machineId);
};

#endif // ADS1115CURRENTSENSORSERVICE_H
