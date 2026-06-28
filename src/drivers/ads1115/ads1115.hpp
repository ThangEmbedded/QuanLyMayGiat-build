#ifndef ADS1115_HPP
#define ADS1115_HPP

#include "i2c.hpp"

class ADS1115 {
public:
    explicit ADS1115(II2c& i2c_impl);
    bool init();
    bool isConnected();
    int readRaw(int channel);
    double readVoltage(int channel);
    bool configure();

private:
    II2c& i2c;
    int getChannelConfig(int channel);
};

#endif // ADS1115_HPP
