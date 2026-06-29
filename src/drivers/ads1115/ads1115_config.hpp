#pragma once

namespace driver::ads1115 {

// Default I2C address for ADS1115
constexpr uint8_t DEFAULT_ADDRESS = 0x48;

// Register addresses
enum class Register : uint8_t {
    CONVERSION = 0x00,
    CONFIG = 0x01,
    LOW_THRESHOLD = 0x02,
    HIGH_THRESHOLD = 0x03
};

// Pointer register bits
namespace Pointer {
    constexpr uint16_t CONVERSION = 0x0000;
    constexpr uint16_t CONFIG = 0x0100;
    constexpr uint16_t LOW_THRESHOLD = 0x0200;
    constexpr uint16_t HIGH_THRESHOLD = 0x0300;
}

// Config register bits
namespace Config {
    // Operational modes
    constexpr uint16_t SINGLE_SHOT = 0x0100;

    // PGA settings
    namespace PGA {
        constexpr uint16_t FS_6_144 = 0x0000;  // ±6.144V
        constexpr uint16_t FS_4_096 = 0x0200;  // ±4.096V
        constexpr uint16_t FS_2_048 = 0x0400;  // ±2.048V (default)
        constexpr uint16_t FS_1_024 = 0x0600;  // ±1.024V
        constexpr uint16_t FS_0_512 = 0x0800;  // ±0.512V
        constexpr uint16_t FS_0_256 = 0x0A00;  // ±0.256V
    }

    // Sample rates
    namespace DataRate {
        constexpr uint16_t SPS_8 = 0x0000;
        constexpr uint16_t SPS_16 = 0x0020;
        constexpr uint16_t SPS_32 = 0x0040;
        constexpr uint16_t SPS_64 = 0x0060;
        constexpr uint16_t SPS_128 = 0x0080; // Default
        constexpr uint16_t SPS_250 = 0x00A0;
        constexpr uint16_t SPS_475 = 0x00C0;
        constexpr uint16_t SPS_860 = 0x00E0;
    }
}

} // namespace driver::ads1115
