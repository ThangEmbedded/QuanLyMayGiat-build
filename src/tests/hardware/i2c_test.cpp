#include "../../drivers/i2c/i2c.hpp"
#include "../../drivers/i2c/i2c_config.hpp"
#include <iostream>
#include <memory>

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

    if (!i2c->deviceExists()) {
        std::cerr << "Device not found" << std::endl;
        i2c->close();
        return 1;
    }

    std::cout << "I2C initialized and device detected" << std::endl;
    i2c->close();
    return 0;
}
