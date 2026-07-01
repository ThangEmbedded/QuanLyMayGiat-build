#include <iostream>
#include "ads1115.hpp"
#include "i2c.hpp"

int main() {
    II2c* i2c = new LinuxI2c();  // Ensure you have a LinuxI2c implementation available
    ADS1115 ads(i2c);

    if (!ads.init()) {
        std::cerr << "Failed to initialize ADS1115." << std::endl;
        return -1;
    }

    uint16_t config = 0xC3; // Example configuration
    if (!ads.configure(config)) {
        std::cerr << "Failed to configure ADS1115." << std::endl;
        return -1;
    }

    int16_t conversionResult;
    if (ads.readConversion(conversionResult)) {
        std::cout << "Conversion Result: " << conversionResult << std::endl;
    } else {
        std::cerr << "Failed to read conversion result." << std::endl;
        return -1;
    }

    delete i2c;
    return 0;
}