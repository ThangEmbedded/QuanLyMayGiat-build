# QuanLyMayGiat - Refactor Plan

Version: 1.0
Author: ChatGPT + Project Owner
Status: Draft
Date: 2026-07-04

---

# 1. Mục tiêu

Refactor dự án theo hướng:

- Clean Architecture
- Embedded Friendly
- Hardware Independent UI
- Dễ mở rộng
- Dễ test
- Không duplicate source code

Project sẽ trở thành nền tảng cho:

- Máy giặt
- Máy sấy
- Thanh toán
- RFID
- MQTT
- OTA
- Remote Monitor

---

# 2. Đánh giá hiện trạng

## Điểm mạnh

Project hiện tại đã có nền tảng rất tốt.

### Driver Layer

```
src/drivers
    gpio
    relay
    i2c
    ads1115
```

Được tách riêng.

Đây là điểm cộng rất lớn.

---

### UI

```
src/ui
```

Đã sử dụng Qt Widget.

Có:

- Controller
- Models
- Pages
- Widgets

Cấu trúc khá rõ ràng.

---

### Test

Đã có

```
tests/
```

để test phần cứng.

Đây là hướng đúng.

---

# 3. Các vấn đề hiện tại

## 3.1 Demo bị copy source

Hiện tại:

```
demo_1

demo_2

demo_3
```

đều là copy UI.

Điều này gây:

- khó maintain
- khó merge
- bug phải sửa nhiều nơi

Đây là vấn đề lớn nhất cần xử lý.

---

## 3.2 UI đang phụ thuộc Hardware

Ví dụ:

MachineController

↓

Relay

↓

GPIO

↓

Hardware

Flow này đúng.

Tuy nhiên các demo đang tự implement thêm tầng relay.

Điều này không cần thiết.

---

## 3.3 services chưa được sử dụng

```
src/services
```

đang để trống.

Cần xác định:

Có dùng Service Layer hay không.

Đề xuất:

Hiện tại KHÔNG cần.

Driver đã đủ abstraction.

---

## 3.4 App chưa có entry point rõ ràng

Hiện

```
src/ui/main.cpp
```

được dùng luôn.

Đề xuất

```
src/app
```

để chứa application.

---

# 4. Kiến trúc đề xuất

```
                    +----------------+
                    |      UI        |
                    +----------------+
                           |
                           |
                    MachineController
                           |
        +------------------+-------------------+
        |                                      |
        |                                      |
    Machine Model                      Relay Driver
                                              |
                                         GPIO Driver
                                              |
                                         Relay Hardware
```

Controller là nơi xử lý business logic.

UI chỉ hiển thị.

Driver chỉ điều khiển hardware.

---

# 5. Source Tree sau Refactor

```
src

├── app
│
├── common
│
├── drivers
│
├── ui
│
└── services
```

Tests

```
tests

├── demo_ui

├── demo_relay

├── demo_full

└── hardware
```

---

# 6. Quy tắc Dependency

Allowed

UI

↓

Controller

↓

Driver

↓

Hardware

Không được phép:

UI

↓

GPIO

UI

↓

Relay

UI chỉ được nói chuyện với Controller.

---

# 7. Demo Strategy

Không copy source.

Ví dụ

```
tests/demo_full
```

chỉ gồm

```
main.cpp

DemoConfig.hpp

README.md

CMakeLists.txt
```

Build sẽ link trực tiếp

```
src/ui

src/drivers
```

---

# 8. Machine State

```
EMPTY

↓

RUNNING

↓

FINISHED

↓

EMPTY
```

EMPTY

- Relay OFF

RUNNING

- Relay ON

FINISHED

- Relay OFF

- Blink Yellow

- Sau timeout quay về EMPTY

---

# 9. Relay Strategy

Không tạo driver mới.

Không dùng sysfs.

Không dùng libgpiod.

Sử dụng trực tiếp

```
src/drivers/relay
```

và

```
src/drivers/gpio
```

đã có.

---

# 10. UI Strategy

Giữ nguyên Qt Widget.

Không chuyển sang QML.

Bổ sung:

- Numeric Keyboard
- Animation
- Status Ring
- Admin Login
- Payment
- Running Status

---

# 11. Coding Rule

Không duplicate source.

Không hardcode GPIO.

Không hardcode Machine Count.

Toàn bộ config sẽ nằm trong

```
Config.hpp
```

hoặc

```
MachineConfig.hpp
```

---

# 12. Demo Plan

## Demo 1

UI Only

Không Relay

Không Hardware

---

## Demo 2

UI

+

State Machine

+

Animation

---

## Demo 3

UI

+

Relay

+

GPIO

---

## Demo 4

Relay

+

ADS1115

+

Current Detect

---

## Demo 5

Full Machine

Relay

ADS1115

Payment

Admin

RFID

---

# 13. Refactor Task List

## Phase 1

- [ ] Loại bỏ duplicate UI
- [ ] Chuyển demo thành Integration App
- [ ] Refactor CMake
- [ ] Tách Config

---

## Phase 2

- [ ] Refactor MachineController
- [ ] Chuẩn hóa State Machine
- [ ] Thêm Event System

---

## Phase 3

- [ ] Tích hợp Relay Driver
- [ ] Tích hợp ADS1115
- [ ] Machine Detection

---

## Phase 4

- [ ] EEPROM
- [ ] Admin Password
- [ ] Persistent Config

---

## Phase 5

- [ ] Payment
- [ ] QR
- [ ] RFID
- [ ] MQTT
- [ ] OTA

---

# 14. Nguyên tắc phát triển

1. Driver chỉ điều khiển Hardware.

2. Controller xử lý Business Logic.

3. UI không biết Hardware.

4. Demo không copy Source.

5. Driver là duy nhất.

6. Test độc lập.

7. Mọi thay đổi UI đều cập nhật qua UI_SPEC.md.

8. Mọi thay đổi Business Logic đều cập nhật qua STATE_MACHINE.md.

9. Mọi thay đổi Driver đều cập nhật Driver README.

10. Mỗi Phase đều build chạy được trên Raspberry Pi.