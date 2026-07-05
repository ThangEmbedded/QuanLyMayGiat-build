#ifndef ADS1115_HPP
#define ADS1115_HPP

#include "../i2c/i2c.hpp"

#include <cstdint>

class ADS1115
{
public:
    explicit ADS1115(II2c& i2c_impl, uint8_t address = 0x48);

    bool init();
    bool isConnected();

    int readRaw(int channel);
    double readVoltage(int channel);
    bool configure();

    uint8_t address() const;

private:
    II2c& i2c;
    uint8_t m_address{0x48};

    bool writeRegister(uint8_t reg, uint16_t value);
    bool readRegister(uint8_t reg, uint16_t& value);
    static bool isValidChannel(int channel);
    static uint16_t getChannelConfig(int channel);
};

#endif // ADS1115_HPP
