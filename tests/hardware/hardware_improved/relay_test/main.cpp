#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "relay.hpp"

namespace
{
using relay::RelayDriver;
using relay::RelayId;

struct RelayCase
{
    RelayId id;
    const char* name;
};

constexpr RelayCase RELAYS[] = {
    {RelayId::MG1, "MG1"},
    {RelayId::MG2, "MG2"},
    {RelayId::MG3, "MG3"},
    {RelayId::MG4, "MG4"},
};

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

bool testInitDefaultOff(RelayDriver& driver)
{
    bool ok = true;
    ok &= expect(driver.init(), "TC01 init relay driver");

    for (const RelayCase& relay : RELAYS)
    {
        ok &= expect(!driver.isOn(relay.id), "TC02 relay defaults to OFF after init");
        std::cout << "  " << relay.name << " OFF\n";
    }

    return ok;
}

bool testOnOff(RelayDriver& driver, int holdMs)
{
    bool ok = true;

    for (const RelayCase& relay : RELAYS)
    {
        std::cout << "\n" << relay.name << " on/off test\n";
        ok &= expect(driver.on(relay.id), "TC03 turn relay ON");
        ok &= expect(driver.isOn(relay.id), "TC04 cached state is ON");
        std::this_thread::sleep_for(std::chrono::milliseconds(holdMs));

        ok &= expect(driver.off(relay.id), "TC05 turn relay OFF");
        ok &= expect(!driver.isOn(relay.id), "TC06 cached state is OFF");
        std::this_thread::sleep_for(std::chrono::milliseconds(holdMs));
    }

    return ok;
}

bool testToggle(RelayDriver& driver, int holdMs)
{
    bool ok = true;

    for (const RelayCase& relay : RELAYS)
    {
        std::cout << "\n" << relay.name << " toggle test\n";
        ok &= expect(driver.off(relay.id), "TC07 force OFF before toggle");
        ok &= expect(driver.toggle(relay.id), "TC08 toggle OFF -> ON");
        ok &= expect(driver.isOn(relay.id), "TC09 state after first toggle is ON");
        std::this_thread::sleep_for(std::chrono::milliseconds(holdMs));

        ok &= expect(driver.toggle(relay.id), "TC10 toggle ON -> OFF");
        ok &= expect(!driver.isOn(relay.id), "TC11 state after second toggle is OFF");
        std::this_thread::sleep_for(std::chrono::milliseconds(holdMs));
    }

    return ok;
}

bool testInvalidId(RelayDriver& driver)
{
    const RelayId invalid = RelayId::Count;
    bool ok = true;

    ok &= expect(!driver.on(invalid), "TC12 invalid id on() rejected");
    ok &= expect(!driver.off(invalid), "TC13 invalid id off() rejected");
    ok &= expect(!driver.toggle(invalid), "TC14 invalid id toggle() rejected");
    ok &= expect(!driver.isOn(invalid), "TC15 invalid id isOn() returns false");

    return ok;
}
} // namespace

int main(int argc, char* argv[])
{
    const int cycles = (argc >= 2) ? parsePositiveInt(argv[1], 1) : 1;
    const int holdMs = (argc >= 3) ? parsePositiveInt(argv[2], 300) : 300;

    std::cout << "=== Relay Hardware Test ===\n";
    std::cout << "Cycles: " << cycles << ", hold: " << holdMs << " ms\n";

    RelayDriver driver;
    bool ok = true;

    ok &= testInitDefaultOff(driver);
    if (ok)
    {
        for (int i = 0; i < cycles; ++i)
        {
            std::cout << "\n--- Cycle " << (i + 1) << '/' << cycles << " ---\n";
            ok &= testOnOff(driver, holdMs);
            ok &= testToggle(driver, holdMs);
        }
        ok &= testInvalidId(driver);
    }

    for (const RelayCase& relay : RELAYS)
    {
        driver.off(relay.id);
    }
    driver.deinit();

    std::cout << "\nRESULT: " << (ok ? "PASS" : "FAIL") << '\n';
    return ok ? 0 : 1;
}
