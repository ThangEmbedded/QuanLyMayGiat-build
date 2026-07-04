# demo_3 - Laundry Kiosk UI + Relay Driver thật

`demo_3` là bản demo chạy thực tế trên Raspberry Pi, đặt trong thư mục `tests/demo_3` của project `QuanLyMayGiat`.

Mục tiêu chính:

- Giữ UI flow từ `demo_2`.
- Thêm popup bàn phím số khi nhập phòng và mật khẩu admin.
- Gọi trực tiếp driver relay thật trong `src/drivers/relay`.
- Không dùng `/sys/class/gpio` tự viết riêng trong UI.
- Không dùng `libgpiod`.
- Relay active HIGH: mức HIGH là bật relay.

## Cấu trúc thư mục

```text
QuanLyMayGiat/
├── src/
│   └── drivers/
│       ├── gpio/
│       │   ├── gpio.cpp
│       │   ├── gpio.hpp
│       │   └── gpio_types.hpp
│       └── relay/
│           ├── relay_config.hpp
│           ├── relay.cpp
│           └── relay.hpp
└── tests/
    └── demo_3/
        ├── CMakeLists.txt
        ├── README.md
        ├── UI_SPEC.md
        └── ui/
            ├── CMakeLists.txt
            ├── main.cpp
            ├── MainWindow.*
            ├── controllers/
            ├── models/
            ├── pages/
            ├── widgets/
            └── services/
                ├── IRelayService.h
                ├── MockRelayService.*
                └── RealRelayService.*
```

## Kiến trúc implement

```text
Qt UI
  -> MainWindow
  -> MachineController
  -> IRelayService
  -> RealRelayService
  -> relay::RelayDriver trong src/drivers/relay/relay.hpp
  -> GPIO driver trong src/drivers/gpio
  -> Relay hardware
```

`MachineController` không gọi driver relay trực tiếp. Controller chỉ gọi interface `IRelayService`, nhờ vậy UI không phụ thuộc chi tiết hardware.

## Mapping relay

```text
Máy 1 -> relay::RelayId::MG1
Máy 2 -> relay::RelayId::MG2
Máy 3 -> relay::RelayId::MG3
Máy 4 -> relay::RelayId::MG4
```

Pin relay thực tế lấy từ driver/config có sẵn trong:

```text
src/drivers/relay/relay_config.hpp
```

## Flow relay

Khi user chọn máy trống và xác nhận khởi động:

```text
ConfirmStartPage
  -> MachineController::startMachine(machineId, room)
  -> RealRelayService::turnOn(machineId)
  -> relay::RelayDriver::on(RelayId)
  -> UI chuyển sang trạng thái Đang giặt
```

Khi hết thời gian giặt, reset máy hoặc chuyển offline:

```text
MachineController
  -> RealRelayService::turnOff(machineId)
  -> relay::RelayDriver::off(RelayId)
```

Khi app khởi động, `RealRelayService::initialize()` gọi `RelayDriver::init()` rồi tắt toàn bộ 4 relay để đảm bảo trạng thái an toàn.

## Build trên Raspberry Pi

Copy hoặc giải nén `demo_3` vào đúng vị trí:

```text
QuanLyMayGiat/tests/demo_3
```

Build:

```bash
cd ~/work_data/1_projects/QuanLyMayGiat/tests/demo_3/ui
mkdir -p build
cd build
cmake .. -DUSE_REAL_RELAY=ON
make -j4
```

Chạy binary:

```bash
./laundry_kiosk_demo_3
```

Nếu chạy kiosk qua framebuffer:

```bash
sudo env -u XDG_RUNTIME_DIR ./laundry_kiosk_demo_3 -platform linuxfb
```

## Build không relay để test UI trên PC

```bash
cd tests/demo_3/ui
mkdir -p build
cd build
cmake .. -DUSE_REAL_RELAY=OFF
make -j4
./laundry_kiosk_demo_3
```

Khi `USE_REAL_RELAY=OFF`, app dùng `MockRelayService`.

## Ghi chú quan trọng

- `demo_3` không tự include driver relay riêng trong UI.
- `RealRelayService` include `drivers/relay/relay.hpp` từ thư mục `src` của project.
- CMake add trực tiếp source:
  - `src/drivers/gpio/gpio.cpp`
  - `src/drivers/relay/relay.cpp`
- Nếu API relay driver thay đổi, chỉ cần sửa `ui/services/RealRelayService.*`, không cần sửa UI pages.
