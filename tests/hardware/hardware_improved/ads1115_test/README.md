# ADS1115 hardware test

Purpose: smoke-test the real ADS1115 device through the Linux I2C driver.

Build:

```bash
cmake -S . -B build -DDRIVER_ROOT=/path/to/src/drivers
cmake --build build
```

Run:

```bash
sudo ./build/ads1115_hardware_test /dev/i2c-1 3
```

Notes:
- This is a hardware integration test, not a pure unit test.
- It expects ADS1115 at the address configured in the driver.
- The test checks init, connectivity, default configuration, channel reads, and invalid channel handling.
