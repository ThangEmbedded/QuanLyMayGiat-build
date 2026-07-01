#include "i2c.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockI2c : public II2c {
public:
    MOCK_METHOD(bool, init, (), (override));
    MOCK_METHOD(bool, writeRead, (uint8_t*, size_t, uint8_t*, size_t), (override));
};

class I2CTest : public ::testing::Test {
protected:
    MockI2c mockI2c;
    II2c* i2c;
    
    void SetUp() override {
        i2c = &mockI2c;
    }
};

TEST_F(I2CTest, InitCallsI2cInit) {
    EXPECT_CALL(mockI2c, init()).WillOnce(::testing::Return(true));
    ASSERT_TRUE(i2c->init());
}

TEST_F(I2CTest, WriteReadCalls) {
    uint8_t data[2] = {0x01, 0x02};
    EXPECT_CALL(mockI2c, writeRead(data, 2, nullptr, 0)).WillOnce(::testing::Return(true));
    ASSERT_TRUE(i2c->writeRead(data, 2, nullptr, 0));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}