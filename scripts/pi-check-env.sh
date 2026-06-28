#!/bin/bash
# =============================================================
# Script kiểm tra môi trường trên Raspberry Pi 4
# Chạy trên Pi: bash pi-check-env.sh
# =============================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

PASS=0
FAIL=0
WARN=0

pass() { echo -e "  ${GREEN}✓ PASS${NC}: $1"; ((PASS++)); }
fail() { echo -e "  ${RED}✗ FAIL${NC}: $1"; ((FAIL++)); }
warn() { echo -e "  ${YELLOW}⚠ WARN${NC}: $1"; ((WARN++)); }
info() { echo -e "  ${CYAN}ℹ INFO${NC}: $1"; }

echo ""
echo "========================================"
echo "  Kiểm tra môi trường Raspberry Pi 4"
echo "  cho Laundry Kiosk (Qt6 Widgets)"
echo "========================================"
echo ""

# ----- 1. SYSTEM INFO -----
echo "── 1. Thông tin hệ thống ──"

# Architecture
ARCH=$(uname -m)
info "Kiến trúc: $ARCH"
if [[ "$ARCH" == "aarch64" ]]; then
    pass "Đang chạy 64-bit OS (aarch64)"
elif [[ "$ARCH" == "armv7l" ]]; then
    warn "Đang chạy 32-bit OS (armv7l) - khuyến nghị dùng 64-bit"
else
    fail "Kiến trúc không nhận dạng: $ARCH"
fi

# OS info
if [ -f /etc/os-release ]; then
    . /etc/os-release
    info "OS: $PRETTY_NAME"
fi

# Kernel
info "Kernel: $(uname -r)"

# RAM
TOTAL_RAM=$(free -m | awk '/^Mem:/{print $2}')
info "RAM: ${TOTAL_RAM}MB"
if [ "$TOTAL_RAM" -ge 2000 ]; then
    pass "RAM >= 2GB"
else
    warn "RAM < 2GB, có thể thiếu bộ nhớ khi chạy"
fi

# Disk space
AVAIL_DISK=$(df -m / | awk 'NR==2{print $4}')
info "Dung lượng trống: ${AVAIL_DISK}MB"
if [ "$AVAIL_DISK" -ge 2000 ]; then
    pass "Dung lượng trống >= 2GB"
else
    warn "Dung lượng trống < 2GB"
fi

echo ""

# ----- 2. QT6 -----
echo "── 2. Kiểm tra Qt6 ──"

# Qt6 packages
QT6_PKGS=(
    "qt6-base-dev"
    "libqt6widgets6"
    "libqt6gui6"
    "libqt6core6"
)

QT6_MISSING=()
for pkg in "${QT6_PKGS[@]}"; do
    if dpkg -s "$pkg" &>/dev/null; then
        VERSION=$(dpkg -s "$pkg" 2>/dev/null | grep '^Version:' | awk '{print $2}')
        pass "$pkg ($VERSION)"
    else
        fail "$pkg chưa cài"
        QT6_MISSING+=("$pkg")
    fi
done

# Check qmake6
if command -v qmake6 &>/dev/null; then
    info "qmake6: $(qmake6 --version 2>&1 | tail -1)"
fi

echo ""

# ----- 3. GRAPHICS & DISPLAY -----
echo "── 3. Kiểm tra Graphics & Display ──"

# DRM devices
if [ -d /dev/dri ]; then
    pass "/dev/dri tồn tại"
    ls -la /dev/dri/ 2>/dev/null | while read line; do
        info "  $line"
    done
else
    fail "/dev/dri không tồn tại - cần bật KMS driver"
fi

# Check if user is in video group
if id -nG | grep -qw video; then
    pass "User thuộc group 'video'"
else
    fail "User KHÔNG thuộc group 'video' → chạy: sudo usermod -aG video $USER"
fi

# Check if user is in render group
if id -nG | grep -qw render; then
    pass "User thuộc group 'render'"
else
    warn "User KHÔNG thuộc group 'render' → chạy: sudo usermod -aG render $USER"
fi

# Check framebuffer
if [ -e /dev/fb0 ]; then
    pass "/dev/fb0 tồn tại (framebuffer)"
    FB_SIZE=$(cat /sys/class/graphics/fb0/virtual_size 2>/dev/null)
    if [ -n "$FB_SIZE" ]; then
        info "Framebuffer size: $FB_SIZE"
    fi
else
    warn "/dev/fb0 không tồn tại"
fi

# Check display env
if [ -n "$DISPLAY" ]; then
    info "DISPLAY=$DISPLAY (X11 đang chạy)"
elif [ -n "$WAYLAND_DISPLAY" ]; then
    info "WAYLAND_DISPLAY=$WAYLAND_DISPLAY (Wayland đang chạy)"
else
    info "Không có DISPLAY/WAYLAND - sẽ cần dùng EGLFS hoặc LinuxFB"
fi

# EGL/GL libraries
echo ""
echo "── 4. Kiểm tra GL/EGL Libraries ──"

GL_LIBS=(
    "libgl1-mesa-dev"
    "libgles2-mesa-dev"
    "libegl1-mesa-dev"
)

for pkg in "${GL_LIBS[@]}"; do
    if dpkg -s "$pkg" &>/dev/null; then
        pass "$pkg"
    else
        warn "$pkg chưa cài (có thể cần cho EGLFS)"
    fi
done

echo ""

# ----- 5. INPUT / TOUCHSCREEN -----
echo "── 5. Kiểm tra Input/Touchscreen ──"

INPUT_PKGS=(
    "libinput-dev"
    "libxkbcommon-dev"
    "libts-dev"
)

for pkg in "${INPUT_PKGS[@]}"; do
    if dpkg -s "$pkg" &>/dev/null; then
        pass "$pkg"
    else
        warn "$pkg chưa cài"
    fi
done

# Check input devices
if [ -d /dev/input ]; then
    TOUCH_COUNT=$(find /dev/input -name "event*" 2>/dev/null | wc -l)
    info "Tìm thấy $TOUCH_COUNT input event devices"
    if command -v libinput &>/dev/null; then
        info "Danh sách input devices:"
        sudo libinput list-devices 2>/dev/null | grep -E "^Device:|^Capabilities:" | head -20
    fi
else
    warn "/dev/input không tồn tại"
fi

echo ""

# ----- 6. XCB DEPENDENCIES (cho X11 platform) -----
echo "── 6. Kiểm tra XCB dependencies (X11) ──"

XCB_PKGS=(
    "libxcb1-dev"
    "libxcb-cursor-dev"
    "libxcb-glx0-dev"
    "libxcb-keysyms1-dev"
    "libxcb-image0-dev"
    "libxcb-shm0-dev"
    "libxcb-icccm4-dev"
    "libxcb-xfixes0-dev"
    "libxcb-shape0-dev"
    "libxcb-randr0-dev"
    "libxcb-render-util0-dev"
    "libxcb-xkb-dev"
    "libxkbcommon-x11-dev"
)

XCB_MISSING=()
for pkg in "${XCB_PKGS[@]}"; do
    if dpkg -s "$pkg" &>/dev/null; then
        pass "$pkg"
    else
        warn "$pkg chưa cài"
        XCB_MISSING+=("$pkg")
    fi
done

echo ""

# ----- 7. FONT -----
echo "── 7. Kiểm tra Font ──"

if fc-list 2>/dev/null | grep -qi "noto"; then
    pass "Noto fonts đã cài"
else
    warn "Noto fonts chưa cài → sudo apt install fonts-noto"
fi

FONT_COUNT=$(fc-list 2>/dev/null | wc -l)
info "Tổng số font: $FONT_COUNT"

echo ""

# ----- 8. NETWORK -----
echo "── 8. Kiểm tra Network ──"

IP_ADDR=$(hostname -I 2>/dev/null | awk '{print $1}')
if [ -n "$IP_ADDR" ]; then
    pass "IP: $IP_ADDR"
else
    warn "Không tìm thấy IP - kiểm tra kết nối mạng"
fi

# SSH
if systemctl is-active --quiet ssh 2>/dev/null || systemctl is-active --quiet sshd 2>/dev/null; then
    pass "SSH đang chạy"
else
    fail "SSH không chạy → sudo systemctl enable --now ssh"
fi

echo ""

# ----- SUMMARY -----
echo "========================================"
echo "  KẾT QUẢ"
echo "========================================"
echo -e "  ${GREEN}✓ PASS${NC}: $PASS"
echo -e "  ${RED}✗ FAIL${NC}: $FAIL"
echo -e "  ${YELLOW}⚠ WARN${NC}: $WARN"
echo ""

if [ "$FAIL" -gt 0 ]; then
    echo -e "${RED}Có $FAIL lỗi cần sửa trước khi chạy app!${NC}"
    echo ""
fi

# Suggest install commands
ALL_MISSING=("${QT6_MISSING[@]}" "${XCB_MISSING[@]}")
if [ ${#ALL_MISSING[@]} -gt 0 ]; then
    echo "── Lệnh cài đặt các package thiếu ──"
    echo ""
    echo "sudo apt update"
    echo "sudo apt install -y ${ALL_MISSING[*]}"
    echo ""
fi

echo "── Lệnh cài toàn bộ (nếu muốn cài tất cả) ──"
echo ""
echo "sudo apt update && sudo apt install -y \\"
echo "  qt6-base-dev libqt6widgets6 libqt6gui6 libqt6core6 \\"
echo "  libgl1-mesa-dev libgles2-mesa-dev libegl1-mesa-dev \\"
echo "  libxkbcommon-dev libinput-dev libts-dev \\"
echo "  libfontconfig1-dev libfreetype6-dev \\"
echo "  libxcb1-dev libxcb-cursor-dev libxcb-glx0-dev \\"
echo "  libxcb-keysyms1-dev libxcb-image0-dev libxcb-shm0-dev \\"
echo "  libxcb-icccm4-dev libxcb-xfixes0-dev libxcb-shape0-dev \\"
echo "  libxcb-randr0-dev libxcb-render-util0-dev libxcb-xkb-dev \\"
echo "  libxkbcommon-x11-dev fonts-noto"
echo ""
echo "========================================"
