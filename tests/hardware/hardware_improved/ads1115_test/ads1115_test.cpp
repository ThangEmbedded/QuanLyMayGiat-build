#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <thread>

#include "ads1115.hpp"
#include "i2c.hpp"

namespace
{
void printUsage(const char* app)
{
    std::cout << "Usage: " << app << " [bus] [samples_per_channel]\n"
              << "Example: " << app << " /dev/i2c-1 5\n";
}

bool expect(bool condition, const char* name)
{
    std::cout << "[" << (condition ? "PASS" : "FAIL") << "] " << name << '\n';
    return condition;
}

int parsePositiveInt(const char* text, int fallback)
{
    if (text == nullptr)
    {
        return fallback;
    }

    const int value = std::atoi(text);
    return (value > 0) ? value : fallback;
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
    const int samples = (argc >= 3) ? parsePositiveInt(argv[2], 3) : 3;

    std::cout << "=== ADS1115 Hardware Test ===\n";
    std::cout << "Bus: " << busPath << "\n";
    std::cout << "Samples/channel: " << samples << "\n";

    LinuxI2c i2c;
    bool ok = true;

    ok &= expect(i2c.open(busPath), "TC00 open I2C bus");
    if (!i2c.isOpen())
    {
        std::cout << "RESULT: FAIL\n";
        return 1;
    }

    ADS1115 ads(i2c);
    ok &= expect(ads.init(), "TC01 init ADS1115");
    ok &= expect(ads.isConnected(), "TC02 ADS1115 is connected");
    ok &= expect(ads.configure(), "TC03 configure ADS1115 default mode");

    for (int channel = 0; channel < 4; ++channel)
    {
        std::cout << "\nChannel AIN" << channel << '\n';
        for (int sample = 0; sample < samples; ++sample)
        {
            const auto rawOpt = ads.readRawSingleEnded(channel);
            const auto voltageOpt = rawOpt.has_value()
                    ? std::optional<double>(static_cast<double>(*rawOpt) * (4.096 / 32768.0))
                    : std::nullopt;

            const int raw = rawOpt.value_or(0);
            const double voltage = voltageOpt.value_or(0.0);

            const bool rawInRange = rawOpt.has_value() && (raw >= -32768) && (raw <= 32767);
            ok &= expect(rawInRange, "TC04 raw value is int16 range");

            std::cout << "  sample " << (sample + 1)
                      << ": raw=" << raw
                      << ", voltage=" << std::fixed << std::setprecision(6) << voltage << " V\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }

    ok &= expect(!ads.readRawSingleEnded(-1).has_value(), "TC05 invalid negative channel rejected");
    ok &= expect(!ads.readRawSingleEnded(4).has_value(), "TC06 invalid high channel rejected");

    i2c.close();

    std::cout << "\nRESULT: " << (ok ? "PASS" : "FAIL") << '\n';
    return ok ? 0 : 1;
}
