# Demo 3 - UI Demo 2 + Real Relay Driver

Demo này là bản integration chạy UI mới giống `demo_2`, nhưng không copy source UI.

## Kiến trúc

```text
tests/demo_3/main.cpp
    |
    v
src/ui/MainWindow
    |
    v
src/ui/controllers/MachineController
    |
    v
src/ui/services/RelayHardwareService
    |
    v
src/drivers/relay/RelayDriver
    |
    v
src/drivers/gpio/Gpio
```

## Điểm quan trọng

- UI lấy trực tiếp từ `src/ui`.
- Giao diện đã đồng bộ lại theo `demo_2`:
  - Home control 3 màn flow.
  - Admin auth.
  - Confirm start.
  - Numeric input dialog.
  - Status ring: xám/xanh/vàng.
- Relay thật dùng driver có sẵn trong `src/drivers/relay`.
- Relay active HIGH.

## Build

```bash
cd tests/demo_3
mkdir -p build
cd build
cmake ..
make -j4
```

## Run trên Pi qua systemd/linuxfb

```bash
sudo ./laundry_kiosk_demo_3 -platform linuxfb
```
