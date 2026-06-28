#include "../../drivers/ads1115/ads1115.hpp"
#include "../../drivers/i2c/i2c.hpp"
#include "../../drivers/i2c/i2c_config.hpp"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

int main() {
    std::unique_ptr<II2c> i2c = std::make_unique<LinuxI2c>();
    if (!i2c->init()) {
        std::cerr << "Failed to initialize I2C" << std::endl;
        return 1;
    }
    if (!i2c->open(I2C_BUS)) {
        std::cerr << "Failed to open I2C bus" << std::endl;
        return 1;
    }

    ADS1115 adc(*i2c);
    if (!adc.isConnected()) {
        std::cerr << "ADS1115 not found" << std::endl;
        i2c->close();
        return 1;
    }

    adc.init();
    while (true) {
        for (int ch = 0; ch < 4; ++ch) {
            std::cout << "Channel " << ch << " Voltage: " << adc.readVoltage(ch) << " V" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    i2c->close();
    return 0;
}
