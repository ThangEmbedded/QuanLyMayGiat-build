# ADS1115 Hardware Test

## Overview

This test verifies the functionality of the ADS1115 driver using a mock I2C interface. It checks:

- Initialization
- Configuration
- Reading conversion results

## Execution

1. Build the test using the provided CMakeLists.txt.
2. Run the executable `ads1115_test`.
3. Observe the console for pass/fail messages.

Expect the following outputs:
- Initialization succeeded
- Configuration succeeded
- Conversion result: (a mock value)

## Purpose

The test ensures that the ADS1115 functions are called correctly and that the driver interacts with the I2C layer as expected. It uses a mock to simulate the I2C hardware, allowing for isolated testing of the driver logic without physical hardware.
