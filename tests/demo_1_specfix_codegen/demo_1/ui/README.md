# Demo 1 - Laundry Kiosk UI

Demo này chỉ tập trung 3 màn chính:

1. **Home control máy giặt**
   - Xem 4 máy giặt
   - Chọn máy còn trống
   - Vào Admin setup

2. **Điều khiển / chọn chương trình máy giặt**
   - Chọn chu kỳ giặt
   - Xem thời gian và giá demo
   - Đi tiếp sang xác nhận bật máy

3. **Admin setup**
   - Bật/tắt máy mock
   - Reset máy đang chạy
   - Xem log thao tác

Màn xác nhận bật máy là bước trung gian của flow Home -> Control -> Confirm -> Home.
Hiện demo chỉ dùng `MockHardwareService`, chưa tích hợp relay/ADS1115.

## Build

```bash
cd ui
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
./laundry_kiosk_demo_1
```
