#include "gpio.hpp"

#include <gpiod.h>

#include <map>
#include <mutex>

namespace gpio
{
namespace
{
constexpr const char* kDefaultChipPath = "/dev/gpiochip0";
constexpr const char* kConsumerName = "may_giat_gpio";

struct PinContext
{
    gpiod_line_request* request{nullptr};
    Direction direction{Direction::Input};
};

gpiod_chip* chip = nullptr;
std::map<unsigned int, PinContext> pin_contexts;
std::mutex gpio_mutex;

void releasePinContext(PinContext& context)
{
    if (context.request != nullptr) {
        gpiod_line_request_release(context.request);
        context.request = nullptr;
    }
}

bool requestLine(unsigned int pin, Direction direction)
{
    if (chip == nullptr) {
        return false;
    }

    auto existing = pin_contexts.find(pin);
    if (existing != pin_contexts.end()) {
        if (existing->second.direction == direction) {
            return true;
        }

        releasePinContext(existing->second);
        pin_contexts.erase(existing);
    }

    gpiod_line_settings* settings = gpiod_line_settings_new();
    if (settings == nullptr) {
        return false;
    }

    if (direction == Direction::Output) {
        gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
        gpiod_line_settings_set_drive(settings, GPIOD_LINE_DRIVE_PUSH_PULL);
        gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);
    } else {
        gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
        gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_UP);
    }

    gpiod_line_config* line_cfg = gpiod_line_config_new();
    if (line_cfg == nullptr) {
        gpiod_line_settings_free(settings);
        return false;
    }

    gpiod_line_config_add_line_settings(line_cfg, &pin, 1, settings);

    gpiod_request_config* req_cfg = gpiod_request_config_new();
    if (req_cfg == nullptr) {
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);
        return false;
    }

    gpiod_request_config_set_consumer(req_cfg, kConsumerName);

    gpiod_line_request* request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);

    gpiod_request_config_free(req_cfg);
    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);

    if (request == nullptr) {
        return false;
    }

    pin_contexts[pin] = PinContext{request, direction};
    return true;
}
} // namespace

Gpio::~Gpio()
{
    deinit();
}

bool Gpio::init()
{
    std::lock_guard<std::mutex> lock(gpio_mutex);

    if (chip != nullptr) {
        return true;
    }

    chip = gpiod_chip_open(kDefaultChipPath);
    return chip != nullptr;
}

void Gpio::deinit()
{
    std::lock_guard<std::mutex> lock(gpio_mutex);

    for (auto& [pin, context] : pin_contexts) {
        (void)pin;
        releasePinContext(context);
    }
    pin_contexts.clear();

    if (chip != nullptr) {
        gpiod_chip_close(chip);
        chip = nullptr;
    }
}

bool Gpio::setOutput(unsigned int pin)
{
    std::lock_guard<std::mutex> lock(gpio_mutex);
    return requestLine(pin, Direction::Output);
}

bool Gpio::setInput(unsigned int pin)
{
    std::lock_guard<std::mutex> lock(gpio_mutex);
    return requestLine(pin, Direction::Input);
}

bool Gpio::write(unsigned int pin, Level level)
{
    std::lock_guard<std::mutex> lock(gpio_mutex);

    auto it = pin_contexts.find(pin);
    if (it == pin_contexts.end() || it->second.direction != Direction::Output || it->second.request == nullptr) {
        return false;
    }

    const gpiod_line_value value = (level == Level::High) ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
    return gpiod_line_request_set_values_subset(it->second.request, 1, &pin, &value) == 0;
}

Level Gpio::read(unsigned int pin)
{
    std::lock_guard<std::mutex> lock(gpio_mutex);

    auto it = pin_contexts.find(pin);
    if (it == pin_contexts.end() || it->second.direction != Direction::Input || it->second.request == nullptr) {
        return Level::Low;
    }

    gpiod_line_value value = GPIOD_LINE_VALUE_INACTIVE;
    if (gpiod_line_request_get_values_subset(it->second.request, 1, &pin, &value) < 0) {
        return Level::Low;
    }

    return (value == GPIOD_LINE_VALUE_ACTIVE) ? Level::High : Level::Low;
}

bool Gpio::isInitialized() const
{
    std::lock_guard<std::mutex> lock(gpio_mutex);
    return chip != nullptr;
}

bool Gpio::isOutput(unsigned int pin) const
{
    std::lock_guard<std::mutex> lock(gpio_mutex);
    auto it = pin_contexts.find(pin);
    return it != pin_contexts.end() && it->second.direction == Direction::Output;
}

bool Gpio::isInput(unsigned int pin) const
{
    std::lock_guard<std::mutex> lock(gpio_mutex);
    auto it = pin_contexts.find(pin);
    return it != pin_contexts.end() && it->second.direction == Direction::Input;
}

} // namespace gpio
