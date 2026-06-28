#include "gpio.hpp"
#include <gpiod.h>
#include <map>

namespace gpio
{

static gpiod_chip* chip = nullptr;
static std::map<unsigned int, gpiod_line*> lines;

bool Gpio::init()
{
    chip = gpiod_chip_open_by_name("gpiochip0");
    return chip != nullptr;
}

bool Gpio::setOutput(unsigned int pin)
{
    gpiod_line* line = gpiod_chip_get_line(chip, pin);

    if(!line) return false;

    if(gpiod_line_request_output(line, "relay", 0) < 0)
        return false;

    lines[pin] = line;
    return true;
}

bool Gpio::setInput(unsigned int pin)
{
    gpiod_line* line = gpiod_chip_get_line(chip, pin);

    if(!line) return false;

    if(gpiod_line_request_input(line, "relay") < 0)
        return false;

    lines[pin] = line;
    return true;
}

bool Gpio::write(unsigned int pin, Level level)
{
    auto it = lines.find(pin);
    if(it == lines.end()) return false;

    return gpiod_line_set_value(
        it->second,
        level == Level::High ? 1 : 0
    ) == 0;
}

Level Gpio::read(unsigned int pin)
{
    auto it = lines.find(pin);
    if(it == lines.end()) return Level::Low;

    int v = gpiod_line_get_value(it->second);

    return v == 1 ? Level::High : Level::Low;
}

}