#include "relay.hpp"
#include "relay_config.hpp"

#include "../gpio/gpio.hpp"

namespace relay
{
namespace
{
gpio::Gpio gpio_driver;
}

bool RelayDriver::isValid(RelayId id)
{
    return id >= RelayId::MG1 && id < RelayId::Count;
}

unsigned int RelayDriver::getPin(RelayId id)
{
    switch (id) {
        case RelayId::MG1: return RELAY_MG1;
        case RelayId::MG2: return RELAY_MG2;
        case RelayId::MG3: return RELAY_MG3;
        case RelayId::MG4: return RELAY_MG4;
        case RelayId::Count: break;
    }

    return RELAY_MG1;
}

bool RelayDriver::init()
{
    if (initialized_) {
        return true;
    }

    if (!gpio_driver.init()) {
        return false;
    }

    constexpr RelayId relays[] = {RelayId::MG1, RelayId::MG2, RelayId::MG3, RelayId::MG4};

    for (RelayId relay : relays) {
        if (!gpio_driver.setOutput(getPin(relay))) {
            return false;
        }
    }

    for (RelayId relay : relays) {
        if (!set(relay, false)) {
            return false;
        }
    }

    initialized_ = true;
    return true;
}

void RelayDriver::deinit()
{
    if (!initialized_) {
        return;
    }

    (void)off(RelayId::MG1);
    (void)off(RelayId::MG2);
    (void)off(RelayId::MG3);
    (void)off(RelayId::MG4);

    gpio_driver.deinit();
    initialized_ = false;
}

bool RelayDriver::set(RelayId id, bool enabled)
{
    if (!isValid(id)) {
        return false;
    }

    const gpio::Level level = enabled ? gpio::Level::High : gpio::Level::Low;
    if (!gpio_driver.write(getPin(id), level)) {
        return false;
    }

    relay_state_[static_cast<int>(id)] = enabled;
    return true;
}

bool RelayDriver::on(RelayId id)
{
    if (!initialized_) {
        return false;
    }

    return set(id, true);
}

bool RelayDriver::off(RelayId id)
{
    if (!initialized_) {
        return false;
    }

    return set(id, false);
}

bool RelayDriver::toggle(RelayId id)
{
    if (!initialized_ || !isValid(id)) {
        return false;
    }

    return set(id, !relay_state_[static_cast<int>(id)]);
}

bool RelayDriver::isOn(RelayId id) const
{
    if (!isValid(id)) {
        return false;
    }

    return relay_state_[static_cast<int>(id)];
}

} // namespace relay
