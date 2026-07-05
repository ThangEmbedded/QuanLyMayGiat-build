#include "ads1115.hpp"
#include "ads1115_config.hpp"

#include <chrono>
#include <cstdint>
#include <thread>

namespace
{
constexpr uint8_t kConversionRegister = 0x00;
constexpr uint8_t kConfigRegister = 0x01;

constexpr uint16_t kOsStartSingleConversion = 0x8000;
constexpr uint16_t kPga4096 = 0x0200;
constexpr uint16_t kModeSingleShot = 0x0100;
constexpr uint16_t kDataRate128Sps = 0x0080;
constexpr uint16_t kComparatorDisabled = 0x0003;

constexpr uint16_t kMuxDiff0_1 = 0x0000;
constexpr uint16_t kMuxDiff0_3 = 0x1000;
constexpr uint16_t kMuxDiff1_3 = 0x2000;
constexpr uint16_t kMuxDiff2_3 = 0x3000;
constexpr uint16_t kMuxSingleEnded0 = 0x4000;
constexpr uint16_t kMuxSingleEnded1 = 0x5000;
constexpr uint16_t kMuxSingleEnded2 = 0x6000;
constexpr uint16_t kMuxSingleEnded3 = 0x7000;

constexpr double kFullScaleVoltage = 4.096;
constexpr double kAdcCounts = 32768.0;
constexpr auto kConversionDelay = std::chrono::milliseconds(10);
}

ADS1115::ADS1115(II2c& i2c_impl) : i2c(i2c_impl) {}

bool ADS1115::init()
{
    if (!i2c.setSlave(ADS1115_I2C_ADDRESS)) {
        return false;
    }

    return configure();
}

bool ADS1115::isConnected()
{
    if (!i2c.setSlave(ADS1115_I2C_ADDRESS)) {
        return false;
    }

    uint16_t config = 0;
    return readRegister(kConfigRegister, config);
}

std::optional<int16_t> ADS1115::readRawSingleEnded(int channel)
{
    if (!isValidChannel(channel)) {
        return std::nullopt;
    }
    return readRawWithMux(getSingleEndedMuxConfig(channel));
}

std::optional<int16_t> ADS1115::readRawDifferential(DifferentialPair pair)
{
    return readRawWithMux(getDifferentialMuxConfig(pair));
}

std::optional<double> ADS1115::readVoltageSingleEnded(int channel)
{
    const auto rawValue = readRawSingleEnded(channel);
    if (!rawValue.has_value()) {
        return std::nullopt;
    }

    return static_cast<double>(*rawValue) * (kFullScaleVoltage / kAdcCounts);
}

std::optional<double> ADS1115::readVoltageDifferential(DifferentialPair pair)
{
    const auto rawValue = readRawDifferential(pair);
    if (!rawValue.has_value()) {
        return std::nullopt;
    }

    return static_cast<double>(*rawValue) * (kFullScaleVoltage / kAdcCounts);
}

int ADS1115::readRaw(int channel)
{
    const auto rawValue = readRawSingleEnded(channel);
    return rawValue.has_value() ? static_cast<int>(*rawValue) : -1;
}

double ADS1115::readVoltage(int channel)
{
    const auto voltage = readVoltageSingleEnded(channel);
    return voltage.has_value() ? *voltage : -1.0;
}

bool ADS1115::configure()
{
    if (!i2c.setSlave(ADS1115_I2C_ADDRESS)) {
        return false;
    }

    // Default safe single-shot single-ended configuration for AIN0.
    return writeRegister(kConfigRegister, buildConfig(kMuxSingleEnded0));
}

bool ADS1115::writeRegister(uint8_t reg, uint16_t value)
{
    const uint8_t tx[3] = {
        reg,
        static_cast<uint8_t>((value >> 8) & 0xFF),
        static_cast<uint8_t>(value & 0xFF)
    };

    return i2c.write(tx, sizeof(tx));
}

bool ADS1115::readRegister(uint8_t reg, uint16_t& value)
{
    const uint8_t tx[1] = {reg};
    uint8_t rx[2] = {0, 0};

    if (!i2c.writeRead(tx, sizeof(tx), rx, sizeof(rx))) {
        return false;
    }

    value = static_cast<uint16_t>((static_cast<uint16_t>(rx[0]) << 8) | rx[1]);
    return true;
}

std::optional<int16_t> ADS1115::readRawWithMux(uint16_t muxConfig)
{
    if (!i2c.setSlave(ADS1115_I2C_ADDRESS)) {
        return std::nullopt;
    }

    if (!writeRegister(kConfigRegister, buildConfig(muxConfig))) {
        return std::nullopt;
    }

    std::this_thread::sleep_for(kConversionDelay);

    uint16_t raw = 0;
    if (!readRegister(kConversionRegister, raw)) {
        return std::nullopt;
    }

    return static_cast<int16_t>(raw);
}

bool ADS1115::isValidChannel(int channel)
{
    return channel >= 0 && channel <= 3;
}

uint16_t ADS1115::getSingleEndedMuxConfig(int channel)
{
    switch (channel) {
        case 0: return kMuxSingleEnded0;
        case 1: return kMuxSingleEnded1;
        case 2: return kMuxSingleEnded2;
        case 3: return kMuxSingleEnded3;
        default: return kMuxSingleEnded0;
    }
}

uint16_t ADS1115::getDifferentialMuxConfig(DifferentialPair pair)
{
    switch (pair) {
        case DifferentialPair::Ain0Ain1: return kMuxDiff0_1;
        case DifferentialPair::Ain0Ain3: return kMuxDiff0_3;
        case DifferentialPair::Ain1Ain3: return kMuxDiff1_3;
        case DifferentialPair::Ain2Ain3: return kMuxDiff2_3;
    }

    return kMuxDiff0_1;
}

uint16_t ADS1115::buildConfig(uint16_t muxConfig)
{
    return static_cast<uint16_t>(kOsStartSingleConversion |
                                 muxConfig |
                                 kPga4096 |
                                 kModeSingleShot |
                                 kDataRate128Sps |
                                 kComparatorDisabled);
}
