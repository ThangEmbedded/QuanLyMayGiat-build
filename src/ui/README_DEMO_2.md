# demo_2

Demo 2 kế thừa demo_1 và bổ sung:

1. Tầng Relay
   - Interface: `services/IRelayService.h`
   - Mock implementation: `services/MockRelayService.*`
   - Controller gọi relay khi start/finish/reset/offline máy.

2. Bàn phím số trong app
   - Widget: `widgets/NumericInputDialog.*`
   - Dùng cho nhập phòng và mật khẩu admin.
   - Không phụ thuộc virtual keyboard của Raspberry Pi OS.

Build:

```bash
cd ui
mkdir -p build
cd build
cmake ..
make -j4
./laundry_kiosk_demo_2 -platform linuxfb
```
