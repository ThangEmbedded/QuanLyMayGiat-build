# ADS1115 Driver Module

## Purpose

This module is an ADC driver for the ADS1115 using I2C abstraction.

## Architecture

Built on top of the II2c interface to conform to the project's architecture standards.

## Public API

- `init()`
- `isConnected()`
- `readRaw(channel)`
- `readVoltage(channel)`
- `configure()`

## Hardware Connection

- I2C Address: `0x48`

## Usage Example

Create an `ADS1115` object with an existing `II2c` instance.

## Test Procedure

Execute with corresponding hardware test files using the `LinuxI2c` implementation.

## Known Limitations

Ensure channel configurations match actual ADS1115 settings.