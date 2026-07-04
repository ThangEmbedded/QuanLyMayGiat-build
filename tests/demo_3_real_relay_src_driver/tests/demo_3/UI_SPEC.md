# Demo 1 - UI Flow Specification

## 1. Mục tiêu
Demo UI cho hệ thống quản lý máy giặt, tập trung vào 3 flow chính:

1. Home control máy giặt
2. Xác thực và xác nhận khởi động máy giặt
3. Admin setup có xác thực mật khẩu

File này là nguồn yêu cầu chính. Khi cần chỉnh UI/flow, sửa nội dung trong file spec này rồi gửi lại để update source.

---

## 2. Danh sách màn hình

### 2.1 HomePage - Home control máy giặt

**Mục đích:** hiển thị trạng thái các máy giặt và cho phép chọn máy trống để khởi động.

**Bố cục giữ nguyên:**
- Header: `Home control máy giặt`
- Badge: số máy trống
- Nút: `Admin setup`
- Grid card máy giặt: 4 máy

**Hành vi:**
- Bấm `Admin setup` -> chuyển sang `AdminAuthPage`
- Bấm card máy có trạng thái `Open` -> chuyển sang `ConfirmStartPage`
- Bấm card máy không phải `Open` -> không làm gì

---

### 2.2 AdminAuthPage - Xác thực admin

**Mục đích:** yêu cầu mật khẩu trước khi vào cài đặt admin.

**UI:**
- Header có nút quay lại nhưng không hiển thị chữ “Xác thực Admin” trên trang
- Không hiển thị mật khẩu mặc định
- Ô nhập mật khẩu
- Nút xác nhận
- Nút quay lại

**Logic:**
- Mật khẩu đúng -> vào `AdminDashboardPage`
- Mật khẩu sai -> hiển thị popup “Sai mật khẩu”
- Popup tự đóng sau 5 giây
- Sau khi popup đóng -> tự động quay về `HomePage`

---

### 2.3 AdminDashboardPage - Admin setup

**Mục đích:** cấu hình/bật tắt/reset máy giặt.

**Yêu cầu hiện tại:** giữ nguyên giao diện admin setup hiện có.

**Hành vi:**
- Quay lại -> về `HomePage`
- Toggle máy -> online/offline
- Reset máy -> về trạng thái trống

---

### 2.4 ConfirmStartPage - Nhập phòng và xác nhận khởi động

**Mục đích:** xác nhận khởi động máy trống.

**Input:**
- Phòng sử dụng máy
- Nút `Xác nhận khởi động`
- Nút quay lại

**Logic:**
- Nhập phòng hợp lệ -> gọi `startMachine(machineId, cycle, roomNumber)`
- Thành công -> quay về `HomePage`
- Sai input/rỗng -> hiển thị lỗi tại màn xác nhận

---

## 3. State machine máy giặt

### 3.1 Danh sách trạng thái

| State | Tên hiển thị | Màu vòng status | Có thể click từ Home |
|---|---|---|---|
| `Open` | Đang trống | Xám `#8e9994` | Có |
| `Running` | Đang giặt | Xanh dương `#0051ca` | Không |
| `Finished` | Đã giặt xong | Vàng `#f4b400` | Không |
| `Offline` | Tạm ngưng | Xám `#8e9994` | Không |
| `Error` | Lỗi | Đỏ/cảnh báo, chưa dùng trong demo | Không |

---

### 3.2 Chuyển trạng thái

```text
Open
  -> user chọn máy + nhập phòng + xác nhận
Running
  -> sau khi hết thời gian giặt
Finished
  -> giữ trong 2 phút
Open
```

Admin có thể ép trạng thái:

```text
Any state -> Offline
Any state -> Open bằng reset/online
```

---

## 4. UI card máy giặt

### 4.1 Open - Đang trống

**Text:**
- Status: `Đang trống`
- Detail: `Sẵn sàng khởi động`

**Vòng status:**
- Màu xám
- Sáng cố định

---

### 4.2 Running - Đang giặt

**Text:**
- Status: `Đang giặt`
- Detail:

```text
Phòng <room>
Đã giặt <elapsed> phút
```

**Vòng status:**
- Màu xanh dương
- Sáng bình thường
- Hiệu ứng: cứ mỗi 2 phút tắt 1 giây rồi sáng lại

---

### 4.3 Finished - Đã giặt xong

**Text:**
- Status: `Đã giặt xong`
- Detail:

```text
Phòng <room>
Vui lòng lấy đồ
```

**Vòng status:**
- Màu vàng
- Khi vừa hoàn thành: blink 2 lần
- Mỗi blink theo chu kỳ 1 giây
- Sau khi blink: sáng vàng 10 giây
- Sau đó lặp lại blink + sáng 10 giây
- Sau 2 phút ở trạng thái `Finished`, tự động chuyển về `Open`

---

## 5. Tham số dễ chỉnh

| Tên | Giá trị hiện tại | Ý nghĩa |
|---|---:|---|
| `ADMIN_PASSWORD` | `1234` | Mật khẩu admin demo |
| `RUNNING_PULSE_INTERVAL_MS` | `120000` | Running sáng 2 phút |
| `RUNNING_PULSE_OFF_MS` | `1000` | Running tắt 1 giây |
| `FINISHED_BLINK_HALF_STEP_MS` | `500` | Nửa chu kỳ blink |
| `FINISHED_BLINK_COUNT` | `2` | Số lần blink khi đã giặt xong |
| `FINISHED_HOLD_ON_MS` | `10000` | Sáng vàng 10 giây sau blink |
| `FINISHED_AUTO_OPEN_MINUTES` | `2` | Sau 2 phút về trống |
| `OPEN_RING_COLOR` | `#8e9994` | Màu xám trạng thái trống |
| `RUNNING_RING_COLOR` | `#0051ca` | Màu xanh dương đang giặt |
| `FINISHED_RING_COLOR` | `#f4b400` | Màu vàng đã giặt xong |

---

## 6. Gợi ý cách gửi yêu cầu update lần sau

Chỉ cần gửi phần thay đổi theo format này:

```text
Update UI_SPEC.md:
- Đổi mật khẩu admin thành: <password>
- Đổi số máy từ 4 thành: <n>
- Running: đổi blink/tắt/sáng thành: <rule>
- Finished: đổi thời gian giữ vàng thành: <rule>
- Home card: đổi text trạng thái thành: <text>
```



---

# Demo 2 Additions

## Relay Layer

- UI không gọi relay/GPIO trực tiếp.
- `MachineController` gọi qua `IRelayService`.
- PC/demo dùng `MockRelayService`.
- Embedded/Raspberry Pi có thể thay bằng `GpioRelayService` sau.
- Khi xác nhận khởi động máy:
  + `MachineController::startMachine()` kiểm tra máy đang trống.
  + Gọi `relayService->turnOn(machineId)`.
  + Nếu relay bật thất bại: không chuyển máy sang Running.
  + Nếu hardware start thất bại: relay tự tắt lại.
- Khi máy giặt xong, offline hoặc reset:
  + Gọi `relayService->turnOff(machineId)`.

## Numeric Keypad

- Các ô nhập số không phụ thuộc bàn phím hệ điều hành.
- Áp dụng cho:
  + Nhập phòng ở màn xác nhận bật máy.
  + Nhập mật khẩu ở màn admin auth.
- Khi chạm vào ô nhập:
  + Hiện popup bàn phím số trong app.
  + Có nút 0-9, C, Xóa, Hủy, OK.
- Ô mật khẩu hiển thị dạng bullet trong popup và trong input chính.

## Demo 3 - Real Relay Integration

- Demo 3 đặt trong `tests/demo_3`.
- Relay thật được gọi qua `src/drivers/relay/relay.hpp` và `src/drivers/relay/relay.cpp`.
- Không dùng `/sys/class/gpio` tự viết riêng trong UI.
- Không dùng `libgpiod`.
- Relay active HIGH: HIGH là bật relay.
- Mapping:
  + Máy 1 -> `relay::RelayId::MG1`
  + Máy 2 -> `relay::RelayId::MG2`
  + Máy 3 -> `relay::RelayId::MG3`
  + Máy 4 -> `relay::RelayId::MG4`
- App khởi động sẽ init relay driver và tắt toàn bộ relay.
- Khi xác nhận khởi động máy: bật relay tương ứng.
- Khi máy giặt xong/reset/offline: tắt relay tương ứng.
