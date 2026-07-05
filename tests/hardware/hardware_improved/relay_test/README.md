# Relay hardware test

Purpose: smoke-test the real relay output pins through the relay and GPIO drivers.

Build:

```bash
cmake -S . -B build -DDRIVER_ROOT=/path/to/src/drivers
cmake --build build
```

Run:

```bash
sudo ./build/relay_hardware_test 2 500
```

Arguments:
- `cycles`: number of on/off and toggle cycles. Default: 1.
- `hold_ms`: delay after each state change. Default: 300 ms.

Safety:
- The test forces all relays OFF before exit.
- Run only when the connected loads are safe to switch repeatedly.

Scope:
- This is a hardware integration/smoke test, not a pure unit test.
- Unit tests should use a mocked GPIO interface and must not access `/dev/gpiochip*`.
