#pragma once

#include "gpio_types.hpp"

namespace gpio
{

class Gpio
{
public:
    Gpio() = default;
    ~Gpio();

    Gpio(const Gpio&) = delete;
    Gpio& operator=(const Gpio&) = delete;

    bool init();
    void deinit();

    bool setOutput(unsigned int pin);
    bool setInput(unsigned int pin);

    bool write(unsigned int pin, Level level);
    Level read(unsigned int pin);

    bool isInitialized() const;
    bool isOutput(unsigned int pin) const;
    bool isInput(unsigned int pin) const;
};

} // namespace gpio
