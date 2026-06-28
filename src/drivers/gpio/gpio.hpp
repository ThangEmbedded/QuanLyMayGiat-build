#pragma once

#include "gpio_types.hpp"

namespace gpio
{

class Gpio
{
public:

    bool init();

    bool setOutput(unsigned int pin);

    bool setInput(unsigned int pin);

    bool write(unsigned int pin, Level level);

    Level read(unsigned int pin);

};

}