#include "ads1115.hpp"
#include "i2c_mock.hpp"
#include <iostream>

int main() {
    // Mock I2C interface
    MockI2c mockI2c;

    // Create instance of ADS1115 driver
    ADS1115 ads1115(&mockI2c);

    // Initialize the ads1115
    if (!ads1115.init()) {
        std::cerr << "Initialization failed." << std::endl;
        return 1;
    }
    std::cout << "Initialization succeeded." << std::endl;

    // Configure ads1115 with default configuration
    if (!ads1115.configure(0x8583)) { // Example configuration
        std::cerr << "Configuration failed." << std::endl;
        return 1;
    }
    std::cout << "Configuration succeeded." << std::endl;

    // Read conversion result
    int16_t result;
    if (!ads1115.readConversion(result)) {
        std::cerr << "Read conversion failed." << std::endl;
        return 1;
    }
    std::cout << "Conversion result: " << result << std::endl;

    return 0;
}
