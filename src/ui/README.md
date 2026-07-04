# Laundry Kiosk UI

Qt Widgets UI layer for the washing machine management project.

## Current scope

This package is UI-only and uses `MockHardwareService`. It does not call relay, GPIO, I2C, or ADS1115 drivers yet.

## Architecture

```text
ui/
├── MainWindow.*
├── models/          # Shared UI/application models
├── controllers/     # UI state orchestration
├── services/        # Hardware service interface + mock implementation
├── pages/           # Qt pages
├── widgets/         # Reusable Qt widgets
└── resources/       # QSS/resources
```

Runtime flow:

```text
Pages -> MainWindow -> MachineController -> IHardwareService -> MockHardwareService
```

Later integration flow:

```text
Pages -> MainWindow -> MachineController -> HardwareService -> Relay/GPIO/ADS1115 drivers
```

## Build

```bash
cd ui
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
./laundry_kiosk_ui
```
