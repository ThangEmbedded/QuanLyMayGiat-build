#ifndef I2C_HPP
#define I2C_HPP

#include <string_view>
#include <vector>

class II2c {
public:
    virtual bool init() = 0;
    virtual bool open(std::string_view bus) = 0;
    virtual void close() = 0;
    virtual bool read(std::vector<uint8_t>& data) = 0;
    virtual bool write(const std::vector<uint8_t>& data) = 0;
    virtual bool writeRead(const std::vector<uint8_t>& writeData, std::vector<uint8_t>& readData) = 0;
    virtual bool deviceExists() = 0;
    virtual ~II2c() = default;
};

#endif // I2C_HPP
