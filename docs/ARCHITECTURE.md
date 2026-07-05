# ARCHITECTURE

## Layer Architecture

```text
Qt Widgets UI
      |
MachineController
      |
+-----+------------------+
|                        |
Relay Driver       Sensor Drivers
(src/drivers)      (ADS1115...)
      |
GPIO Driver
      |
Hardware
```

### Rules

- UI only handles presentation.
- MachineController owns business logic.
- Drivers are the only layer accessing hardware.
- UI must never call GPIO/Relay directly.

### Dependency

Allowed:

UI -> Controller -> Drivers -> Hardware

Forbidden:

- UI -> Relay
- UI -> GPIO
- Widgets -> Drivers
