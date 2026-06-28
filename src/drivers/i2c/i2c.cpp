#include "i2c.hpp"
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

class LinuxI2c : public II2c {
public:
    bool init() override { 
        return true; 
    }

    bool open(std::string_view bus) override {
        fileDescriptor = ::open(bus.data(), O_RDWR);
        return (fileDescriptor >= 0);
    }

    void close() override {
        if (fileDescriptor >= 0) {
            ::close(fileDescriptor);
        }
    }

    bool read(std::vector<uint8_t>& data) override {
        return (::read(fileDescriptor, data.data(), data.size()) == static_cast<ssize_t>(data.size()));
    }

    bool write(const std::vector<uint8_t>& data) override {
        return (::write(fileDescriptor, data.data(), data.size()) == static_cast<ssize_t>(data.size()));
    }

    bool writeRead(const std::vector<uint8_t>& writeData, std::vector<uint8_t>& readData) override {
        if (!write(writeData)) {
            return false;
        }
        return read(readData);
    }

    bool deviceExists() override {
        return true; // Simplified for demonstration.
    }

private:
    int fileDescriptor = -1;
};
