# relay_test.cpp

## Overview

The `relay_test.cpp` file serves as a unit test for the `RelayDriver` class, which manages the control of various relays in the Laundry Machine Controller project. This test primarily verifies the functionality of turning relays on and off, as well as ensuring that the interactions with the hardware are performed as expected.

## Dependencies

This file includes the `relay.hpp` header file located in the `src/drivers/relay/` directory, which defines the `RelayDriver` class and the necessary relay identifiers.

## Functionality

The main function in the `relay_test.cpp` file demonstrates the following operations:
- **Initialization**: Initializes the `RelayDriver` instance.
- **Relay Operations**: Iterates through a predefined list of relays (Pump, Valve, DoorLock, Heater), performs the following actions for each relay:
  - Turns the relay on for a specified duration (2 seconds).
  - Turns the relay off for a brief period (1 second).

## Test Sequence

During execution, the program will:
1. Print "Relay Test Start" to indicate the beginning of the test.
2. For each relay in the list:
   - Print "ON" when the relay is activated.
   - Wait for 2 seconds to simulate the relay being in the ON state.
   - Print "OFF" when the relay is deactivated.
   - Wait for 1 second to simulate the relay being in the OFF state.
3. Print "Finish" to indicate the end of the test.

## Usage

To execute the relay test, compile the `relay_test.cpp` file within the project's build system and run the resulting executable. The output will indicate whether the relays were properly activated and deactivated during the test.

## Note

This test serves as a foundation for further testing and validation of relay functionality within the application. Additional tests may include error handling and edge cases regarding relay states.