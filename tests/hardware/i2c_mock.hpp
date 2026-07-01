#ifndef I2C_MOCK_HPP
#define I2C_MOCK_HPP

#include "i2c.hpp"
#include <gmock/gmock.h>

class MockI2c : public II2c {
public:
    MOCK_METHOD(bool, init, (), (override));
    MOCK_METHOD(bool, writeRead, (uint8_t*, size_t, uint8_t*, size_t), (override));
};

#endif // I2C_MOCK_HPP