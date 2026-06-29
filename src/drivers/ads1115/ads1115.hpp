#ifndef ADS1115_HPP
#define ADS1115_HPP

#include "i2c.hpp"
#include "ads1115_config.hpp"
#include <cstdint>

class ADS1115 {
public:
    ADS1115(II2c* i2c) : i2c_(i2c) {}
    ~ADS1115() = default;

    // Initialize the device
    bool init();

    // Configure the device with settings
    bool configure(uint16_t config);

    // Read conversion result
    bool readConversion(int16_t &result);

private:
    II2c* i2c_;
};

#endif // ADS1115_HPP
