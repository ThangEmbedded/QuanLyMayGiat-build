#ifndef ADS1115_HPP
#define ADS1115_HPP

#include "../i2c/i2c.hpp"

#include <cstdint>

class ADS1115
{
public:
    explicit ADS1115(II2c& i2c_impl);

    bool init();
    bool isConnected();

    int readRaw(int channel);
    double readVoltage(int channel);
    bool configure();

private:
    II2c& i2c;

    bool writeRegister(uint8_t reg, uint16_t value);
    bool readRegister(uint8_t reg, uint16_t& value);
    static bool isValidChannel(int channel);
    static uint16_t getChannelConfig(int channel);
};

#endif // ADS1115_HPP
