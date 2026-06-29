#include "i2c.hpp"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <unistd.h>

LinuxI2c::~LinuxI2c()
{
    close();
}

bool LinuxI2c::init()
{
    return true;
}

bool LinuxI2c::open(std::string_view bus)
{
    if (isOpen())
    {
        close();
    }

    fd_ = ::open(bus.data(), O_RDWR);

    if (fd_ < 0)
    {
        std::cerr << "I2C open failed : "
                  << bus
                  << " error="
                  << std::strerror(errno)
                  << std::endl;

        return false;
    }

    currentSlave_ = 0xFF;

    return true;
}

void LinuxI2c::close()
{
    if (fd_ >= 0)
    {
        ::close(fd_);
        fd_ = -1;
    }

    currentSlave_ = 0xFF;
}

bool LinuxI2c::isOpen() const
{
    return fd_ >= 0;
}

bool LinuxI2c::setSlave(uint8_t address)
{
    if (!isOpen())
    {
        return false;
    }

    if (currentSlave_ == address)
    {
        return true;
    }

    if (::ioctl(fd_, I2C_SLAVE, address) < 0)
    {
        std::cerr << "I2C set slave failed : 0x"
                  << std::hex
                  << static_cast<int>(address)
                  << " error="
                  << std::strerror(errno)
                  << std::dec
                  << std::endl;

        return false;
    }

    currentSlave_ = address;

    return true;
}

bool LinuxI2c::probe(uint8_t address)
{
    if (!setSlave(address))
    {
        return false;
    }

    return true;
}

bool LinuxI2c::write(const uint8_t* data,
                     size_t length)
{
    if (!isOpen())
    {
        return false;
    }

    if (data == nullptr || length == 0)
    {
        return false;
    }

    ssize_t ret = ::write(fd_, data, length);

    if (ret != static_cast<ssize_t>(length))
    {
        std::cerr << "I2C write failed : "
                  << std::strerror(errno)
                  << std::endl;

        return false;
    }

    return true;
}

bool LinuxI2c::read(uint8_t* data,
                    size_t length)
{
    if (!isOpen())
    {
        return false;
    }

    if (data == nullptr || length == 0)
    {
        return false;
    }

    ssize_t ret = ::read(fd_, data, length);

    if (ret != static_cast<ssize_t>(length))
    {
        std::cerr << "I2C read failed : "
                  << std::strerror(errno)
                  << std::endl;

        return false;
    }

    return true;
}

bool LinuxI2c::writeRead(const uint8_t* tx,
                         size_t txLength,
                         uint8_t* rx,
                         size_t rxLength)
{
    if (!isOpen())
    {
        return false;
    }

    if (tx == nullptr ||
        rx == nullptr ||
        txLength == 0 ||
        rxLength == 0)
    {
        return false;
    }

    struct i2c_msg messages[2];

    messages[0].addr  = currentSlave_;
    messages[0].flags = 0;
    messages[0].len   = static_cast<__u16>(txLength);
    messages[0].buf   = const_cast<uint8_t*>(tx);

    messages[1].addr  = currentSlave_;
    messages[1].flags = I2C_M_RD;
    messages[1].len   = static_cast<__u16>(rxLength);
    messages[1].buf   = rx;

    struct i2c_rdwr_ioctl_data ioctlData;

    ioctlData.msgs = messages;
    ioctlData.nmsgs = 2;

    if (::ioctl(fd_, I2C_RDWR, &ioctlData) < 0)
    {
        std::cerr << "I2C writeRead failed : "
                  << std::strerror(errno)
                  << std::endl;

        return false;
    }

    return true;
}