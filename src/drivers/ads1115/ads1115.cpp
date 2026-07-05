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

constexpr uint16_t kMuxSingleEnded0 = 0x4000;
constexpr uint16_t kMuxSingleEnded1 = 0x5000;
constexpr uint16_t kMuxSingleEnded2 = 0x6000;
constexpr uint16_t kMuxSingleEnded3 = 0x7000;

constexpr double kFullScaleVoltage = 4.096;
constexpr double kAdcCounts = 32768.0;
constexpr auto kConversionDelay = std::chrono::milliseconds(10);
}

ADS1115::ADS1115(II2c& i2c_impl, uint8_t address)
    : i2c(i2c_impl),
      m_address(address)
{
}

bool ADS1115::init()
{
    if (!i2c.setSlave(m_address)) {
        return false;
    }

    return configure();
}

bool ADS1115::isConnected()
{
    return i2c.probe(m_address);
}

int ADS1115::readRaw(int channel)
{
    if (!isValidChannel(channel)) {
        return -1;
    }

    if (!i2c.setSlave(m_address)) {
        return -1;
    }

    const uint16_t config = getChannelConfig(channel);
    if (!writeRegister(kConfigRegister, config)) {
        return -1;
    }

    std::this_thread::sleep_for(kConversionDelay);

    uint16_t raw = 0;
    if (!readRegister(kConversionRegister, raw)) {
        return -1;
    }

    return static_cast<int>(static_cast<int16_t>(raw));
}

double ADS1115::readVoltage(int channel)
{
    const int rawValue = readRaw(channel);
    if (rawValue == -1) {
        return -1.0;
    }

    return static_cast<double>(rawValue) * (kFullScaleVoltage / kAdcCounts);
}

bool ADS1115::configure()
{
    if (!i2c.setSlave(m_address)) {
        return false;
    }

    return writeRegister(kConfigRegister, getChannelConfig(0));
}

uint8_t ADS1115::address() const
{
    return m_address;
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

bool ADS1115::isValidChannel(int channel)
{
    return channel >= 0 && channel <= 3;
}

uint16_t ADS1115::getChannelConfig(int channel)
{
    uint16_t mux = kMuxSingleEnded0;

    switch (channel) {
        case 0: mux = kMuxSingleEnded0; break;
        case 1: mux = kMuxSingleEnded1; break;
        case 2: mux = kMuxSingleEnded2; break;
        case 3: mux = kMuxSingleEnded3; break;
        default: mux = kMuxSingleEnded0; break;
    }

    return static_cast<uint16_t>(kOsStartSingleConversion |
                                 mux |
                                 kPga4096 |
                                 kModeSingleShot |
                                 kDataRate128Sps |
                                 kComparatorDisabled);
}
