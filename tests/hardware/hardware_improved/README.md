# Hardware driver tests

This folder contains hardware integration/smoke tests for the driver layer.

Current modules:

- `relay_test`: validates relay on/off/toggle behavior on the real GPIO hardware.
- `i2c_test`: validates Linux I2C open/probe/read behavior.
- `ads1115_test`: validates ADS1115 init/connect/read flow through I2C.

Important distinction:

- These are **hardware tests**, because they touch Linux devices such as `/dev/i2c-*` and `/dev/gpiochip*`.
- They are not pure unit tests. Pure unit tests should live separately, for example under `unit/`, and use mock objects instead of Linux drivers.

Recommended test layout for the project:

```text
tests/
  unit/
    relay/
    ads1115/
    i2c/
  hardware/
    relay_test/
    i2c_test/
    ads1115_test/
```

Build example from each test folder:

```bash
cmake -S . -B build -DDRIVER_ROOT=/absolute/path/to/src/drivers
cmake --build build
```

Do not commit generated `build/` folders.
