#include "relay.hpp"
#include "relay_config.hpp"

#include "../gpio/gpio.hpp"

using namespace gpio;

namespace relay
{

static Gpio gpio;

static unsigned int getPin(RelayId id)
{
    switch(id)
    {
        case RelayId::MG1:
            return RELAY_MG1;

        case RelayId::MG2:
            return RELAY_MG2;

        case RelayId::MG3:
            return RELAY_MG3;

        case RelayId::MG4:
            return RELAY_MG4;
    }

    return RELAY_MG1;
}

bool RelayDriver::init()
{
    gpio.init();

    gpio.setOutput(RELAY_MG1);
    gpio.setOutput(RELAY_MG2);
    gpio.setOutput(RELAY_MG3);
    gpio.setOutput(RELAY_MG4);

    return true;
}

bool RelayDriver::on(RelayId id)
{
    return gpio.write(getPin(id), Level::High);
}

bool RelayDriver::off(RelayId id)
{
    return gpio.write(getPin(id), Level::Low);
}

bool RelayDriver::toggle(RelayId id)
{
    auto pin = getPin(id);

    auto state = gpio.read(pin);

    return gpio.write(
            pin,
            state == Level::High
                ? Level::Low
                : Level::High);
}

}