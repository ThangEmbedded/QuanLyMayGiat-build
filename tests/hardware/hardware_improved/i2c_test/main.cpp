#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "i2c.hpp"

namespace
{
constexpr uint8_t ADS1115_CONFIG_REG = 0x01U;

uint8_t parseAddress(const std::string& text)
{
    unsigned long value = std::stoul(text, nullptr, 0);
    if (value > 0x7FU)
    {
        throw std::out_of_range("I2C address must be 7-bit");
    }
    return static_cast<uint8_t>(value);
}

std::vector<uint8_t> defaultAds1115Addresses()
{
    return {0x48U, 0x49U, 0x4AU, 0x4BU};
}

void printUsage(const char* app)
{
    std::cout << "Usage: " << app << " [bus] [addr ...]\n"
              << "Example: " << app << " /dev/i2c-1 0x48 0x49\n";
}

bool expect(bool condition, const char* name)
{
    std::cout << "[" << (condition ? "PASS" : "FAIL") << "] " << name << '\n';
    return condition;
}

bool testOpenClose(const std::string& busPath)
{
    LinuxI2c bus;
    bool ok = true;

    ok &= expect(!bus.isOpen(), "TC01.01 bus is closed before open");
    ok &= expect(bus.open(busPath), "TC01.02 open valid bus");
    ok &= expect(bus.isOpen(), "TC01.03 bus is open after open()");
    bus.close();
    ok &= expect(!bus.isOpen(), "TC01.04 bus is closed after close()");

    return ok;
}

bool testInvalidParameters(LinuxI2c& bus)
{
    bool ok = true;
    uint8_t rx[2] = {};
    const uint8_t tx[1] = {ADS1115_CONFIG_REG};

    ok &= expect(!bus.setSlave(0x80U), "TC02.01 reject invalid 8-bit address");
    ok &= expect(!bus.write(nullptr, 1U), "TC02.02 reject null write buffer");
    ok &= expect(!bus.write(tx, 0U), "TC02.03 reject zero write length");
    ok &= expect(!bus.read(nullptr, 1U), "TC02.04 reject null read buffer");
    ok &= expect(!bus.read(rx, 0U), "TC02.05 reject zero read length");
    ok &= expect(!bus.writeRead(nullptr, 1U, rx, sizeof(rx)), "TC02.06 reject null tx in writeRead");
    ok &= expect(!bus.writeRead(tx, sizeof(tx), nullptr, sizeof(rx)), "TC02.07 reject null rx in writeRead");

    return ok;
}

bool testProbeAndReadConfig(LinuxI2c& bus, const std::vector<uint8_t>& addresses)
{
    bool ok = true;

    for (uint8_t addr : addresses)
    {
        std::cout << "\nDevice 0x" << std::hex << std::uppercase << static_cast<int>(addr) << std::dec << '\n';

        const bool present = bus.probe(addr);
        ok &= expect(present, "TC03.01 probe ADS1115 address");
        if (!present)
        {
            continue;
        }

        uint8_t tx = ADS1115_CONFIG_REG;
        uint8_t rx[2] = {};
        const bool readOk = bus.writeRead(&tx, 1U, rx, sizeof(rx));
        ok &= expect(readOk, "TC03.02 read ADS1115 config register");

        if (readOk)
        {
            std::cout << "Config: 0x"
                      << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(rx[0])
                      << std::setw(2) << static_cast<int>(rx[1])
                      << std::dec << std::setfill(' ') << '\n';
        }
    }

    return ok;
}
} // namespace

int main(int argc, char* argv[])
{
    if (argc > 1 && std::string(argv[1]) == "--help")
    {
        printUsage(argv[0]);
        return 0;
    }

    const std::string busPath = (argc >= 2) ? argv[1] : "/dev/i2c-1";
    std::vector<uint8_t> addresses;

    try
    {
        for (int i = 2; i < argc; ++i)
        {
            addresses.push_back(parseAddress(argv[i]));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Invalid address argument: " << e.what() << '\n';
        printUsage(argv[0]);
        return 2;
    }

    if (addresses.empty())
    {
        addresses = defaultAds1115Addresses();
    }

    std::cout << "=== I2C Hardware Test ===\n";
    std::cout << "Bus: " << busPath << "\n";

    bool ok = true;
    ok &= testOpenClose(busPath);

    LinuxI2c bus;
    ok &= expect(bus.open(busPath), "TC00 open bus for functional tests");
    if (bus.isOpen())
    {
        ok &= testInvalidParameters(bus);
        ok &= testProbeAndReadConfig(bus, addresses);
        bus.close();
    }

    std::cout << "\nRESULT: " << (ok ? "PASS" : "FAIL") << '\n';
    return ok ? 0 : 1;
}
