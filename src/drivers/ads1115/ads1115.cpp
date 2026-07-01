#include "ads1115.hpp"
#include <iostream>
#include <unistd.h>

bool ADS1115::init()
{
    // Initialize the I2C communication
    if (!i2c_->init()) {
        std::cerr << "Failed to initialize I2C." << std::endl;
        return false;
    }
    return true;
}

bool ADS1115::configure(uint16_t config)
{
    uint8_t configData[3];
    configData[0] = static_cast<uint8_t>(config >> 8);
    configData[1] = static_cast<uint8_t>(config & 0xFF);

    if (!i2c_->writeRead(configData, 2, nullptr, 0)) {
        std::cerr << "Failed to write to config register." << std::endl;
        return false;
    }

    return true;
}

bool ADS1115::readConversion(int16_t &result)
{
    uint8_t buffer[2];
    if (!i2c_->writeRead(nullptr, 0, buffer, 2)) {
        std::cerr << "Failed to read conversion result." << std::endl;
        return false;
    }

    result = (buffer[0] << 8) | buffer[1];
    return true;
}
