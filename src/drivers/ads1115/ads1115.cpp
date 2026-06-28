#include "ads1115.hpp"
#include <iostream>

ADS1115::ADS1115(II2c& i2c_impl) : i2c(i2c_impl) {}

bool ADS1115::init() {
    return configure();
}

bool ADS1115::isConnected() {
    return i2c.deviceExists();
}

int ADS1115::readRaw(int channel) {
    int config = getChannelConfig(channel);
    std::vector<uint8_t> writeData = { static_cast<uint8_t>(config >> 8), static_cast<uint8_t>(config & 0xFF) };
    std::vector<uint8_t> readData(2);

    if (!i2c.writeRead(writeData, readData)) {
        return -1;
    }

    return (readData[0] << 8) | readData[1];
}

double ADS1115::readVoltage(int channel) {
    int rawValue = readRaw(channel);
    if (rawValue == -1) {
        return -1.0;
    }
    return rawValue * (4.096 / 32768.0);
}

bool ADS1115::configure() {
    // Configure ADC if needed
    return true;
}

int ADS1115::getChannelConfig(int channel) {
    switch (channel) {
        case 0: return 0xC300; // Config for AIN0
        case 1: return 0xD300; // Config for AIN1
        case 2: return 0xE300; // Config for AIN2
        case 3: return 0xF300; // Config for AIN3
        default: return -1;
    }
}
