#ifndef ADS1115CURRENTSENSORSERVICE_H
#define ADS1115CURRENTSENSORSERVICE_H

#include "ICurrentSensorService.h"
#include "drivers/i2c/i2c.hpp"
#include "drivers/ads1115/ads1115.hpp"

#include <array>
#include <cstdint>
#include <memory>

class Ads1115CurrentSensorService : public ICurrentSensorService {
public:
    // activeSignalThreshold is voltage delta threshold after zero-baseline calibration.
    // Hardware mapping: each machine has its own ADS1115, and all machines use AIN0.
    explicit Ads1115CurrentSensorService(double activeSignalThreshold = 0.12);
    ~Ads1115CurrentSensorService() override;

    bool initialize() override;
    bool hasCurrent(int machineId) override;
    double lastVoltage(int machineId) const override;

private:
    struct SensorBinding {
        int machineId;
        uint8_t address;
        int channel;
    };

    static constexpr int kMachineCount = 4;
    static constexpr int kSamplesPerRead = 8;
    static constexpr int kCalibrationSamples = 16;

    static constexpr std::array<SensorBinding, kMachineCount> kBindings{{
        {1, 0x48, 0}, // U6: ADDR -> GND, machine 1, AIN0
        {2, 0x49, 0}, // U3: ADDR -> VDD, machine 2, AIN0
        {3, 0x4A, 0}, // U4: ADDR -> SDA, machine 3, AIN0
        {4, 0x4B, 0}, // U5: ADDR -> SCL, machine 4, AIN0
    }};

    LinuxI2c m_i2c;
    std::array<std::unique_ptr<ADS1115>, kMachineCount> m_ads{};
    std::array<bool, kMachineCount> m_adsReady{{false, false, false, false}};
    std::array<double, kMachineCount> m_baselines{{0.0, 0.0, 0.0, 0.0}};
    std::array<double, kMachineCount> m_lastVoltages{{0.0, 0.0, 0.0, 0.0}};
    std::array<double, kMachineCount> m_lastSignals{{0.0, 0.0, 0.0, 0.0}};

    bool m_initialized{false};
    double m_activeSignalThreshold{0.12};

    static bool isValidMachineId(int machineId);
    static int indexForMachine(int machineId);
    bool calibrateBaseline(int index);
    bool readSignal(int index, double &averageVoltage, double &maxDeltaFromBaseline);
};

#endif // ADS1115CURRENTSENSORSERVICE_H
