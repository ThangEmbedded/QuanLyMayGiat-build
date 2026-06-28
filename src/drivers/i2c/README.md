# I2C HAL Module

## Purpose

This module provides an abstraction layer for I2C communications on Linux platforms.

## Architecture

Follows the project's hardware abstraction layers with separate interfaces for implementation and mocking.

## Public API

- `init()`
- `open(bus)`
- `close()`
- `read()`
- `write()`
- `writeRead()`
- `deviceExists()`

## Hardware Connection

- Bus: `/dev/i2c-1`
- Default Address: Use from `i2c_config.hpp`

## Usage Example

Create an `II2c` pointer and instantiate `LinuxI2c` or `MockI2c` for usage.

## Test Procedure

Unit tests can be executed without physical hardware using `MockI2c` implementation.

## Known Limitations

Consult project's `DevelopmentStandard.md` for behavior constraints.