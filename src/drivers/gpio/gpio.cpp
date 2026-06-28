#include "gpio.hpp"

#include <gpiod.hpp>
#include <iostream>
#include <map>

namespace gpio
{

static gpiod::chip chip("/dev/gpiochip0");
static std::map<unsigned int, gpiod::line> lines;

bool Gpio::init()
{
    return true;
}

bool Gpio::setOutput(unsigned int pin)
{
    try
    {
        auto line = chip.get_line(pin);

        line.request({
            "LaundryGPIO",
            gpiod::line_request::DIRECTION_OUTPUT,
            0
        });

        lines.emplace(pin, std::move(line));

        return true;
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool Gpio::setInput(unsigned int pin)
{
    try
    {
        auto line = chip.get_line(pin);

        line.request({
            "LaundryGPIO",
            gpiod::line_request::DIRECTION_INPUT,
            0
        });

        lines.emplace(pin, std::move(line));

        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool Gpio::write(unsigned int pin, Level level)
{
    auto it = lines.find(pin);

    if(it == lines.end())
        return false;

    it->second.set_value(level == Level::High);

    return true;
}

Level Gpio::read(unsigned int pin)
{
    auto it = lines.find(pin);

    if(it == lines.end())
        return Level::Low;

    return it->second.get_value()
            ? Level::High
            : Level::Low;
}

}