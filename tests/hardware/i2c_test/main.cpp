#include <iomanip>
#include <iostream>

#include "i2c.hpp"

constexpr char I2C_BUS[] = "/dev/i2c-1";

constexpr uint8_t ADS_ADDRS[] =
{
    0x48,
    0x49,
    0x4A,
    0x4B
};

static bool testOpen(LinuxI2c& bus)
{
    std::cout << "[TC01] Open I2C Bus ... ";

    if (!bus.open(I2C_BUS))
    {
        std::cout << "FAIL\n";
        return false;
    }

    std::cout << "PASS\n";
    return true;
}

static bool testProbe(LinuxI2c& bus)
{
    std::cout << "\n[TC02] Probe Devices\n";

    bool result = true;

    for (uint8_t addr : ADS_ADDRS)
    {
        std::cout << "  0x"
                  << std::hex
                  << std::uppercase
                  << static_cast<int>(addr)
                  << " : ";

        if (bus.probe(addr))
        {
            std::cout << "PASS";
        }
        else
        {
            std::cout << "FAIL";
            result = false;
        }

        std::cout << std::endl;
    }

    std::cout << std::dec;

    return result;
}

static bool testReadConfig(LinuxI2c& bus)
{
    std::cout << "\n[TC03] Read Config Register\n";

    bool result = true;

    constexpr uint8_t REG_CONFIG = 0x01;

    for (uint8_t addr : ADS_ADDRS)
    {
        uint8_t tx = REG_CONFIG;
        uint8_t rx[2] = {};

        std::cout << "  ADS 0x"
                  << std::hex
                  << std::uppercase
                  << static_cast<int>(addr)
                  << " : ";

        if (!bus.setSlave(addr))
        {
            std::cout << "FAIL (setSlave)\n";
            result = false;
            continue;
        }

        if (!bus.writeRead(&tx, 1, rx, 2))
        {
            std::cout << "FAIL (read)\n";
            result = false;
            continue;
        }

        std::cout << "0x"
                  << std::setw(2)
                  << std::setfill('0')
                  << static_cast<int>(rx[0])
                  << " "
                  << "0x"
                  << std::setw(2)
                  << static_cast<int>(rx[1])
                  << '\n';
    }

    std::cout << std::dec;

    return result;
}

int main()
{
    std::cout << "=========================================\n";
    std::cout << "       I2C HAL Hardware Test\n";
    std::cout << "=========================================\n\n";

    LinuxI2c bus;

    bool ok = true;

    ok &= testOpen(bus);
    ok &= testProbe(bus);
    ok &= testReadConfig(bus);

    bus.close();

    std::cout << "\n=========================================\n";

    if (ok)
    {
        std::cout << "RESULT : PASS\n";
        return 0;
    }

    std::cout << "RESULT : FAIL\n";
    return 1;
}