#ifndef I2C_HPP
#define I2C_HPP

#include <cstddef>
#include <cstdint>
#include <mutex>
#include <string>
#include <string_view>

#include "logger.hpp"

class II2c
{
public:
    virtual ~II2c() = default;

    virtual bool init() = 0;
    virtual bool open(std::string_view bus) = 0;
    virtual void close() = 0;

    virtual bool isOpen() const = 0;

    virtual bool setSlave(uint8_t address) = 0;
    virtual bool probe(uint8_t address) = 0;

    virtual bool write(const uint8_t* data, size_t length) = 0;
    virtual bool read(uint8_t* data, size_t length) = 0;

    virtual bool writeRead(const uint8_t* tx, size_t txLength, uint8_t* rx, size_t rxLength) = 0;
};

class LinuxI2c final : public II2c
{
public:
    enum class I2cError
    {
        None,
        NotInitialized,
        OpenFailed,
        CloseFailed,
        InvalidParameter,
        InvalidSlaveAddress,
        IoctlFailed,
        ReadFailed,
        WriteFailed,
        TransferFailed,
        ProbeFailed,
        DeviceNotOpen,
        Unknown
    };

    LinuxI2c() = default;
    ~LinuxI2c() override;

    LinuxI2c(const LinuxI2c&) = delete;
    LinuxI2c& operator=(const LinuxI2c&) = delete;

    bool init() override;
    bool open(std::string_view bus) override;
    void close() override;
    bool isOpen() const override;

    bool isValidSlaveAddress(uint8_t address) const;
    bool setSlave(uint8_t address) override;
    bool probe(uint8_t address) override;

    bool write(const uint8_t* data, size_t length) override;
    bool read(uint8_t* data, size_t length) override;
    bool writeRead(const uint8_t* tx, size_t txLength, uint8_t* rx, size_t rxLength) override;

    I2cError lastError() const;
    std::string lastErrorString() const;

private:
    bool isOpenUnlocked() const;
    bool setSlaveUnlocked(uint8_t address);
    bool writeUnlocked(const uint8_t* data, size_t length);
    bool readUnlocked(uint8_t* data, size_t length);
    void closeUnlocked();
    void setError(I2cError error);

    int fd_{-1};
    uint8_t currentSlave_{0xff};
    I2cError lastError_{I2cError::None};
    mutable std::mutex mutex_;
    Logger logger_;
};

#endif // I2C_HPP
