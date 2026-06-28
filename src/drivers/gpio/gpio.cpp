#include "gpio.hpp"
#include <gpiod.h>
#include <map>
#include <stdexcept>

namespace gpio
{

static struct gpiod_chip* chip = nullptr;
static struct gpiod_line_request* line_request_output = nullptr;
static struct gpiod_line_request* line_request_input = nullptr;

static std::map<unsigned int, bool> initialized_pins;

bool Gpio::init()
{
    chip = gpiod_chip_open("/dev/gpiochip0"); 
    if (!chip) {
        throw std::runtime_error("Failed to open GPIO chip /dev/gpiochip0");
    }
    return true;
}

bool Gpio::setOutput(unsigned int pin)
{
    if (!chip) return false;

    struct gpiod_line_settings* settings = gpiod_line_settings_new();
    if (!settings) return false;

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    
    // ĐÃ CẬP NHẬT: Dùng PUSH_PULL đồng bộ theo file test vừa chạy tốt
    gpiod_line_settings_set_drive(settings, GPIOD_LINE_DRIVE_PUSH_PULL);

    struct gpiod_line_config* line_cfg = gpiod_line_config_new();
    if (!line_cfg) {
        gpiod_line_settings_free(settings);
        return false;
    }

    gpiod_line_config_add_line_settings(line_cfg, &pin, 1, settings);

    struct gpiod_request_config* req_cfg = gpiod_request_config_new();
    if (!req_cfg) {
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);
        return false;
    }
    gpiod_request_config_set_consumer(req_cfg, "relay_may_giat");

    line_request_output = gpiod_chip_request_lines(chip, req_cfg, line_cfg);

    gpiod_request_config_free(req_cfg);
    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);

    if (!line_request_output) {
        return false;
    }

    initialized_pins[pin] = true;
    return true;
}

bool Gpio::setInput(unsigned int pin)
{
    if (!chip) return false;

    struct gpiod_line_settings* settings = gpiod_line_settings_new();
    if (!settings) return false;

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
    gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_UP);

    struct gpiod_line_config* line_cfg = gpiod_line_config_new();
    if (!line_cfg) {
        gpiod_line_settings_free(settings);
        return false;
    }

    gpiod_line_config_add_line_settings(line_cfg, &pin, 1, settings);

    struct gpiod_request_config* req_cfg = gpiod_request_config_new();
    if (!req_cfg) {
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);
        return false;
    }
    gpiod_request_config_set_consumer(req_cfg, "input_may_giat");

    line_request_input = gpiod_chip_request_lines(chip, req_cfg, line_cfg);

    gpiod_request_config_free(req_cfg);
    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);

    if (!line_request_input) {
        return false;
    }

    initialized_pins[pin] = false;
    return true;
}

bool Gpio::write(unsigned int pin, Level level)
{
    if (!line_request_output || initialized_pins.find(pin) == initialized_pins.end()) {
        throw std::runtime_error("Pin not initialized as output");
    }

    enum gpiod_line_value val = (level == Level::High) ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
    
    return gpiod_line_request_set_values_subset(line_request_output, 1, &pin, &val) == 0;
}

Level Gpio::read(unsigned int pin)
{
    if (!line_request_input) {
        throw std::runtime_error("Input lines not initialized");
    }

    enum gpiod_line_value val;
    
    if (gpiod_line_request_get_values_subset(line_request_input, 1, &pin, &val) < 0) {
        throw std::runtime_error("Failed to read value from pin");
    }

    return (val == GPIOD_LINE_VALUE_ACTIVE) ? Level::High : Level::Low;
}

} // namespace gpio