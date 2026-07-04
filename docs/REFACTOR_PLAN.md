# REFACTOR_PLAN

## Goal

Refactor project so demos do not copy UI source code. Demos should be integration apps that build directly from `src/ui` and `src/drivers`.

## Completed in this package

- Added injectable hardware backend to `MainWindow`.
- Added `RelayHardwareService` that implements existing `IHardwareService`.
- `RelayHardwareService` calls existing `src/drivers/relay/RelayDriver`.
- Added `tests/demo_3` as an integration demo.
- `tests/demo_3` does not duplicate UI files.

## New dependency flow

```text
MainWindow
  -> MachineController
  -> IHardwareService
  -> RelayHardwareService
  -> RelayDriver
  -> Gpio
  -> Hardware
```

## Next phases

1. Move shared CMake source list into a common CMake module.
2. Replace demo copies with integration apps.
3. Add persistent config for machine count, password and relay mapping.
4. Add complete UI spec-driven flow for admin setup, keypad and status ring.
