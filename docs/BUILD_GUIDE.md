# Hướng dẫn Build & Chạy - Laundry Kiosk

> Ứng dụng Kiosk quản lý máy giặt, xây dựng bằng **Qt6 Widgets + CMake**, tối ưu cho màn hình cảm ứng **800×480**.

---

## Mục lục

- [1. Yêu cầu chung](#1-yêu-cầu-chung)
- [2. Build & Chạy trên Linux Desktop (Test/Dev)](#2-build--chạy-trên-linux-desktop-testdev)
- [3. Cross-Compile trên PC & Chạy trên Raspberry Pi 4](#3-cross-compile-trên-pc--chạy-trên-raspberry-pi-4)
- [4. Cấu trúc dự án](#4-cấu-trúc-dự-án)
- [5. Xử lý lỗi thường gặp](#5-xử-lý-lỗi-thường-gặp)

---

## 1. Yêu cầu chung

| Thành phần    | Phiên bản tối thiểu |
| ------------- | -------------------- |
| CMake         | 3.14+                |
| C++ Compiler  | GCC 10+ / Clang 12+  |
| Qt            | Qt6 (khuyến nghị) hoặc Qt5 |

---

## 2. Build & Chạy trên Linux Desktop (Test/Dev)

Phần này dùng để **test giao diện trên PC** trong quá trình phát triển.

### 2.1. Cài đặt dependencies

**Ubuntu / Debian:**

```bash
sudo apt update
sudo apt install -y build-essential cmake qt6-base-dev
```

> Nếu distro chưa có Qt6, có thể dùng Qt5:
> ```bash
> sudo apt install -y build-essential cmake qtbase5-dev
> ```

**Fedora:**

```bash
sudo dnf install -y gcc-c++ cmake qt6-qtbase-devel
```

**Arch Linux:**

```bash
sudo pacman -S base-devel cmake qt6-base
```

### 2.2. Build

```bash
cd QuanLyMayGiat

# Build bằng script có sẵn
chmod +x scripts/build.sh
./scripts/build.sh
```

Hoặc build thủ công:

```bash
cmake -B build
cmake --build build -j$(nproc)
```

### 2.3. Chạy trên Desktop

```bash
./build/LaundryKiosk
```

Ứng dụng sẽ mở cửa sổ **800×480** trên desktop để test giao diện.

---

## 3. Cross-Compile trên PC & Chạy trên Raspberry Pi 4

> **Luồng làm việc:** Build trên PC (x86_64) → Copy binary sang Pi 4 (aarch64) → Chạy trên Pi.

### 3.1. Chuẩn bị phần cứng Pi

- Raspberry Pi 4B (2GB RAM trở lên)
- LCD Touch TFT 7" (800×480)
- Thẻ nhớ microSD 16GB+
- OS: **Raspberry Pi OS 64-bit (Bookworm)** hoặc Ubuntu Server 22.04+ (64-bit)

---

### 3.2. Chuẩn bị sysroot từ Pi

Trước khi cross-compile, cần lấy sysroot (thư viện hệ thống) từ Pi để compiler biết link với gì.

#### Bước 1: Cài Qt6 và dependencies trên Pi

SSH vào Pi và cài đặt:

```bash
sudo apt update && sudo apt upgrade -y
sudo apt install -y qt6-base-dev libgl1-mesa-dev libgles2-mesa-dev \
    libxkbcommon-dev libinput-dev libts-dev libfontconfig1-dev \
    libfreetype6-dev libx11-dev libxext-dev libxfixes-dev \
    libxi-dev libxrender-dev libxcb1-dev libxcb-cursor-dev \
    libxcb-glx0-dev libxcb-keysyms1-dev libxcb-image0-dev \
    libxcb-shm0-dev libxcb-icccm4-dev libxcb-sync-dev \
    libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev \
    libxcb-render-util0-dev libxcb-xinerama0-dev libxcb-xkb-dev \
    libxkbcommon-x11-dev
```

#### Bước 2: Rsync sysroot từ Pi về PC

Trên PC host:

```bash
# Tạo thư mục sysroot
mkdir -p ~/pi-sysroot

# Rsync các thư viện cần thiết từ Pi
rsync -avz --rsync-path="sudo rsync" pi@<PI_IP>:/lib/ ~/pi-sysroot/lib/
rsync -avz --rsync-path="sudo rsync" pi@<PI_IP>:/usr/lib/ ~/pi-sysroot/usr/lib/
rsync -avz --rsync-path="sudo rsync" pi@<PI_IP>:/usr/include/ ~/pi-sysroot/usr/include/
rsync -avz --rsync-path="sudo rsync" pi@<PI_IP>:/opt/ ~/pi-sysroot/opt/
```

> Thay `<PI_IP>` bằng địa chỉ IP của Pi (ví dụ: `192.168.1.100`).

#### Bước 3: Fix symlinks trong sysroot

Symlinks trong sysroot vẫn trỏ đến đường dẫn tuyệt đối trên Pi, cần fix lại:

```bash
# Tải script fix symlinks
wget https://raw.githubusercontent.com/nickhudspeth/sysroot-relativelinks/master/sysroot-relativelinks.py
chmod +x sysroot-relativelinks.py
python3 sysroot-relativelinks.py ~/pi-sysroot
```

---

### 3.3. Cài Cross-Compiler trên PC

```bash
sudo apt install -y gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
```

Kiểm tra:

```bash
aarch64-linux-gnu-gcc --version
```

---

### 3.4. Tạo CMake Toolchain File

Tạo file `toolchain-pi4.cmake` trong thư mục dự án:

```cmake
# toolchain-pi4.cmake - Cross-compile cho Raspberry Pi 4 (aarch64)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Cross-compiler
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# Sysroot từ Pi
set(CMAKE_SYSROOT $ENV{HOME}/pi-sysroot)
set(CMAKE_FIND_ROOT_PATH $ENV{HOME}/pi-sysroot)

# Tìm thư viện trong sysroot, headers trong sysroot
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
```

---

### 3.5. Build cho Pi trên PC

```bash
cd QuanLyMayGiat

# Dùng build directory riêng cho Pi
cmake -B build-pi \
    -DCMAKE_TOOLCHAIN_FILE=toolchain-pi4.cmake

cmake --build build-pi -j$(nproc)
```

> Binary sinh ra: `build-pi/LaundryKiosk` (kiến trúc aarch64, **không chạy được trên PC**).

Kiểm tra binary đúng kiến trúc:

```bash
file build-pi/LaundryKiosk
# Expected: ELF 64-bit LSB executable, ARM aarch64, ...
```

---

### 3.6. Deploy binary sang Pi

```bash
# Copy binary sang Pi qua SCP
scp build-pi/LaundryKiosk pi@<PI_IP>:~/LaundryKiosk

# Hoặc dùng rsync (nhanh hơn khi deploy lặp lại)
rsync -avz build-pi/LaundryKiosk pi@<PI_IP>:~/LaundryKiosk
```

---

### 3.7. Chạy trên Pi

SSH vào Pi:

```bash
ssh pi@<PI_IP>
```

#### Chạy trong Desktop Environment (X11/Wayland)

```bash
chmod +x ~/LaundryKiosk
./LaundryKiosk
```

#### Chạy Fullscreen trực tiếp (EGLFS - không cần Desktop)

```bash
./LaundryKiosk -platform eglfs
```

Hoặc đặt biến môi trường:

```bash
export QT_QPA_PLATFORM=eglfs
export QT_QPA_EGLFS_PHYSICAL_WIDTH=154
export QT_QPA_EGLFS_PHYSICAL_HEIGHT=86
./LaundryKiosk
```

#### Chạy với LinuxFB (fallback)

```bash
./LaundryKiosk -platform linuxfb:fb=/dev/fb0:size=800x480
```

---

### 3.8. Script deploy nhanh (tùy chọn)

Tạo file `scripts/deploy-pi.sh` để tự động build + deploy:

```bash
#!/bin/bash
set -e

PI_USER="pi"
PI_IP="192.168.1.100"       # ← Đổi thành IP Pi của bạn
PI_DEST="/home/pi/LaundryKiosk"

echo "=== Cross-compiling cho Pi 4... ==="
cmake -B build-pi -DCMAKE_TOOLCHAIN_FILE=toolchain-pi4.cmake
cmake --build build-pi -j$(nproc)

echo "=== Kiểm tra binary ==="
file build-pi/LaundryKiosk

echo "=== Deploy sang Pi ($PI_IP)... ==="
scp build-pi/LaundryKiosk ${PI_USER}@${PI_IP}:${PI_DEST}

echo "=== Chạy trên Pi... ==="
ssh ${PI_USER}@${PI_IP} "chmod +x ${PI_DEST} && DISPLAY=:0 ${PI_DEST}"
```

Sử dụng:

```bash
chmod +x scripts/deploy-pi.sh
./scripts/deploy-pi.sh
```

---

### 3.9. Cấu hình Kiosk Mode (Tự động chạy khi Pi khởi động)

#### Cách 1: Systemd Service

Tạo file trên Pi: `/etc/systemd/system/laundry-kiosk.service`

```ini
[Unit]
Description=Laundry Kiosk Application
After=multi-user.target

[Service]
Type=simple
User=pi
Environment=QT_QPA_PLATFORM=eglfs
Environment=QT_QPA_EGLFS_PHYSICAL_WIDTH=154
Environment=QT_QPA_EGLFS_PHYSICAL_HEIGHT=86
ExecStart=/home/pi/LaundryKiosk
Restart=always
RestartSec=5

[Install]
WantedBy=multi-user.target
```

Kích hoạt:

```bash
sudo systemctl enable laundry-kiosk.service
sudo systemctl start laundry-kiosk.service
```

#### Cách 2: Autostart trong Desktop Environment

Tạo file trên Pi: `~/.config/autostart/laundry-kiosk.desktop`

```ini
[Desktop Entry]
Type=Application
Name=Laundry Kiosk
Exec=/home/pi/LaundryKiosk
X-GNOME-Autostart-enabled=true
```

---

### 3.10. Cấu hình màn hình cảm ứng trên Pi

Thêm vào `/boot/firmware/config.txt` (Bookworm) hoặc `/boot/config.txt`:

```ini
# LCD Touch 7"
dtoverlay=vc4-kms-v3d
max_framebuffers=2

# Xoay màn hình (nếu cần)
# display_rotate=0   # 0=normal, 1=90°, 2=180°, 3=270°
```

Calibrate touchscreen nếu cần:

```bash
sudo apt install -y xinput-calibrator
xinput_calibrator
```

---

## 4. Cấu trúc dự án

```
QuanLyMayGiat/
├── CMakeLists.txt              # CMake build configuration
├── toolchain-pi4.cmake         # CMake toolchain cho cross-compile Pi 4
├── scripts/
│   ├── build.sh                # Build cho desktop (test)
│   └── deploy-pi.sh            # Build + deploy sang Pi
├── src/
│   ├── main.cpp                # Entry point
│   ├── MainWindow.h/cpp        # Cửa sổ chính, quản lý navigation
│   ├── resources.qrc           # Qt resource file
│   ├── resources/              # Assets (icons, images)
│   ├── widgets/                # Custom widgets
│   │   ├── TransitionStackedWidget   # Chuyển trang với animation
│   │   ├── CircularProgressRing      # Vòng tiến trình
│   │   ├── QRWidget                  # Hiển thị QR code
│   │   ├── HeaderWidget              # Thanh header
│   │   └── MachineCardWidget         # Thẻ máy giặt
│   └── pages/                  # Các trang giao diện
│       ├── HomePage                  # Trang chủ - chọn máy
│       ├── MachineDetailPage         # Chi tiết máy
│       ├── PaymentPage               # Thanh toán (QR)
│       ├── RunningStatusPage         # Trạng thái máy đang chạy
│       ├── AdminLoginPage            # Đăng nhập admin
│       └── AdminDashboardPage        # Bảng điều khiển admin
├── build/                      # Build output cho desktop (git ignored)
├── build-pi/                   # Build output cho Pi (git ignored)
└── docs/
    ├── UI_Spec.md
    ├── BUILD_GUIDE.md           # (File này)
    └── Screens/
```

---

## 5. Xử lý lỗi thường gặp

### Cross-compile: Qt6 không tìm thấy trong sysroot

```
Could not find a package configuration file provided by "Qt6"
```

**Giải pháp:** Chắc chắn đã cài `qt6-base-dev` **trên Pi** và rsync lại sysroot:

```bash
rsync -avz --rsync-path="sudo rsync" pi@<PI_IP>:/usr/lib/ ~/pi-sysroot/usr/lib/
```

### Lỗi "cannot find -lGL"

```bash
# Trên Pi:
sudo apt install -y libgl1-mesa-dev

# Rsync lại sysroot
```

### Binary chạy trên Pi báo "not found" hoặc "No such file"

Kiểm tra dynamic linker:

```bash
# Trên Pi
ldd ~/LaundryKiosk
```

Nếu thiếu thư viện, cài thêm trên Pi:

```bash
sudo apt install -y libqt6widgets6 libqt6gui6 libqt6core6
```

### Lỗi EGLFS trên Pi

```
EGLFS: Failed to open DRM device
```

**Giải pháp:**

```bash
# Thêm user vào group video và render
sudo usermod -aG video,render pi
# Logout rồi login lại

# Kiểm tra DRM device
ls -la /dev/dri/
```

### Màn hình cảm ứng không hoạt động

```bash
# Kiểm tra input devices
sudo libinput list-devices

# Kiểm tra touch event
sudo evtest
```

### App bị đen màn hình trên Pi (EGLFS)

```bash
# Thử dùng linuxfb thay vì eglfs
./LaundryKiosk -platform linuxfb:fb=/dev/fb0:size=800x480

# Hoặc kiểm tra KMS driver trong raspi-config
sudo raspi-config
# -> Advanced Options -> GL Driver -> G2 GL (Fake KMS)
```

---

## Tóm tắt luồng làm việc

```
┌─────────────────────────────────────────────────────┐
│                    PC Host (x86_64)                  │
│                                                     │
│  1. Code & Test:    ./scripts/build.sh              │
│                     ./build/LaundryKiosk             │
│                                                     │
│  2. Cross-compile:  cmake -B build-pi \             │
│                       -DCMAKE_TOOLCHAIN_FILE=...    │
│                     cmake --build build-pi           │
│                                                     │
│  3. Deploy:         scp build-pi/LaundryKiosk       │
│                       pi@<IP>:~/LaundryKiosk        │
└──────────────────────┬──────────────────────────────┘
                       │ SCP / rsync
                       ▼
┌─────────────────────────────────────────────────────┐
│               Raspberry Pi 4 (aarch64)               │
│                                                     │
│  4. Run:            ./LaundryKiosk                   │
│                     (hoặc -platform eglfs)           │
│                                                     │
│  LCD Touch 7" (800×480)                              │
└─────────────────────────────────────────────────────┘
```

---

> **Tài liệu thiết kế UI:** [docs/UI_Spec.md](./UI_Spec.md) | [docs/Screens/LaundryKioskFlow.html](./Screens/LaundryKioskFlow.html)
