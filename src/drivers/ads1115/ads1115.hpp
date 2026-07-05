#ifndef ADS1115_HPP
#define ADS1115_HPP

#include "../i2c/i2c.hpp"

#include <cstdint>
#include <optional>

class ADS1115
{
public:
    enum class InputMode
    {
        SingleEnded,
        Differential
    };

    enum class DifferentialPair
    {
        Ain0Ain1,
        Ain0Ain3,
        Ain1Ain3,
        Ain2Ain3
    };

    explicit ADS1115(II2c& i2c_impl);

    bool init();
    bool isConnected();

    std::optional<int16_t> readRawSingleEnded(int channel);
    std::optional<int16_t> readRawDifferential(DifferentialPair pair);
    std::optional<double> readVoltageSingleEnded(int channel);
    std::optional<double> readVoltageDifferential(DifferentialPair pair);

    // Backward-compatible API. Prefer the optional-returning methods above.
    int readRaw(int channel);
    double readVoltage(int channel);

    bool configure();

private:
    II2c& i2c;

    bool writeRegister(uint8_t reg, uint16_t value);
    bool readRegister(uint8_t reg, uint16_t& value);
    std::optional<int16_t> readRawWithMux(uint16_t muxConfig);

    static bool isValidChannel(int channel);
    static uint16_t getSingleEndedMuxConfig(int channel);
    static uint16_t getDifferentialMuxConfig(DifferentialPair pair);
    static uint16_t buildConfig(uint16_t muxConfig);
};

#endif // ADS1115_HPP
