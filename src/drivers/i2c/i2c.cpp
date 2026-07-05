#include "i2c.hpp"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

LinuxI2c::~LinuxI2c()
{
    close();
}

bool LinuxI2c::init()
{
    std::lock_guard<std::mutex> lock(mutex_);
    setError(I2cError::None);
    return true;
}

bool LinuxI2c::open(std::string_view bus)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (bus.empty()) {
        setError(I2cError::InvalidParameter);
        return false;
    }

    closeUnlocked();

    const std::string bus_path(bus);
    fd_ = ::open(bus_path.c_str(), O_RDWR);
    if (fd_ < 0) {
        setError(I2cError::OpenFailed);
        logger_.log("I2C open failed: " + bus_path + " error=" + std::strerror(errno));
        return false;
    }

    currentSlave_ = 0xff;
    setError(I2cError::None);
    return true;
}

void LinuxI2c::close()
{
    std::lock_guard<std::mutex> lock(mutex_);
    closeUnlocked();
}

void LinuxI2c::closeUnlocked()
{
    if (fd_ >= 0) {
        if (::close(fd_) < 0) {
            setError(I2cError::CloseFailed);
        }
        fd_ = -1;
    }

    currentSlave_ = 0xff;
}

bool LinuxI2c::isOpen() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return isOpenUnlocked();
}

bool LinuxI2c::isOpenUnlocked() const
{
    return fd_ >= 0;
}

bool LinuxI2c::isValidSlaveAddress(uint8_t address) const
{
    return address >= 0x03 && address <= 0x77;
}

bool LinuxI2c::setSlave(uint8_t address)
{
    std::lock_guard<std::mutex> lock(mutex_);
    return setSlaveUnlocked(address);
}

bool LinuxI2c::setSlaveUnlocked(uint8_t address)
{
    if (!isOpenUnlocked()) {
        setError(I2cError::DeviceNotOpen);
        return false;
    }

    if (!isValidSlaveAddress(address)) {
        setError(I2cError::InvalidSlaveAddress);
        return false;
    }

    if (currentSlave_ == address) {
        setError(I2cError::None);
        return true;
    }

    if (::ioctl(fd_, I2C_SLAVE, address) < 0) {
        setError(I2cError::IoctlFailed);
        logger_.log("I2C set slave failed: address=" + std::to_string(address) + " error=" + std::strerror(errno));
        return false;
    }

    currentSlave_ = address;
    setError(I2cError::None);
    return true;
}

bool LinuxI2c::probe(uint8_t address)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (!setSlaveUnlocked(address)) {
        setError(I2cError::ProbeFailed);
        return false;
    }

    uint8_t value = 0;
    const ssize_t result = ::read(fd_, &value, 1);
    if (result < 0) {
        setError(I2cError::ProbeFailed);
        return false;
    }

    setError(I2cError::None);
    return true;
}

bool LinuxI2c::write(const uint8_t* data, size_t length)
{
    std::lock_guard<std::mutex> lock(mutex_);
    return writeUnlocked(data, length);
}

bool LinuxI2c::writeUnlocked(const uint8_t* data, size_t length)
{
    if (!isOpenUnlocked()) {
        setError(I2cError::DeviceNotOpen);
        return false;
    }

    if (data == nullptr || length == 0) {
        setError(I2cError::InvalidParameter);
        return false;
    }

    const ssize_t result = ::write(fd_, data, length);
    if (result != static_cast<ssize_t>(length)) {
        setError(I2cError::WriteFailed);
        return false;
    }

    setError(I2cError::None);
    return true;
}

bool LinuxI2c::read(uint8_t* data, size_t length)
{
    std::lock_guard<std::mutex> lock(mutex_);
    return readUnlocked(data, length);
}

bool LinuxI2c::readUnlocked(uint8_t* data, size_t length)
{
    if (!isOpenUnlocked()) {
        setError(I2cError::DeviceNotOpen);
        return false;
    }

    if (data == nullptr || length == 0) {
        setError(I2cError::InvalidParameter);
        return false;
    }

    const ssize_t result = ::read(fd_, data, length);
    if (result != static_cast<ssize_t>(length)) {
        setError(I2cError::ReadFailed);
        return false;
    }

    setError(I2cError::None);
    return true;
}

bool LinuxI2c::writeRead(const uint8_t* tx, size_t txLength, uint8_t* rx, size_t rxLength)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (!isOpenUnlocked()) {
        setError(I2cError::DeviceNotOpen);
        return false;
    }

    if (tx == nullptr || txLength == 0 || rx == nullptr || rxLength == 0) {
        setError(I2cError::InvalidParameter);
        return false;
    }

    i2c_msg messages[2]{};
    messages[0].addr = currentSlave_;
    messages[0].flags = 0;
    messages[0].len = static_cast<__u16>(txLength);
    messages[0].buf = const_cast<uint8_t*>(tx);

    messages[1].addr = currentSlave_;
    messages[1].flags = I2C_M_RD;
    messages[1].len = static_cast<__u16>(rxLength);
    messages[1].buf = rx;

    i2c_rdwr_ioctl_data ioctl_data{};
    ioctl_data.msgs = messages;
    ioctl_data.nmsgs = 2;

    if (::ioctl(fd_, I2C_RDWR, &ioctl_data) < 0) {
        setError(I2cError::TransferFailed);
        logger_.log("I2C writeRead failed: error=" + std::string(std::strerror(errno)));
        return false;
    }

    setError(I2cError::None);
    return true;
}

LinuxI2c::I2cError LinuxI2c::lastError() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return lastError_;
}

std::string LinuxI2c::lastErrorString() const
{
    std::lock_guard<std::mutex> lock(mutex_);

    switch (lastError_) {
        case I2cError::None: return "None";
        case I2cError::NotInitialized: return "NotInitialized";
        case I2cError::OpenFailed: return "OpenFailed";
        case I2cError::CloseFailed: return "CloseFailed";
        case I2cError::InvalidParameter: return "InvalidParameter";
        case I2cError::InvalidSlaveAddress: return "InvalidSlaveAddress";
        case I2cError::IoctlFailed: return "IoctlFailed";
        case I2cError::ReadFailed: return "ReadFailed";
        case I2cError::WriteFailed: return "WriteFailed";
        case I2cError::TransferFailed: return "TransferFailed";
        case I2cError::ProbeFailed: return "ProbeFailed";
        case I2cError::DeviceNotOpen: return "DeviceNotOpen";
        case I2cError::Unknown: return "Unknown";
    }

    return "Unknown";
}

void LinuxI2c::setError(I2cError error)
{
    lastError_ = error;
}
