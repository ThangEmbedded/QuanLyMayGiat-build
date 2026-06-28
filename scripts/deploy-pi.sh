#!/bin/bash
set -e

PI_USER="toppi"
PI_IP="192.168.1.102"       # ← Đổi thành IP Pi của bạn
PROJECT_DIR="$HOME/work_data/1_projects/QuanLyMayGiat"
PI_DEST="~/QuanLyMayGiat"

echo "=== [1/4] Build trên PC (test) ==="
cd "$PROJECT_DIR"
./scripts/build.sh

echo "=== [2/4] Sync source sang Pi ==="
rsync -avz --exclude='build/' --exclude='build-pi/' --exclude='.git/' --exclude='.aider*' \
    "$PROJECT_DIR/" ${PI_USER}@${PI_IP}:${PI_DEST}/

echo "=== [3/4] Build trên Pi ==="
ssh ${PI_USER}@${PI_IP} "cd ${PI_DEST} && ./scripts/build.sh"

echo "=== [4/4] Chạy trên Pi ==="
ssh ${PI_USER}@${PI_IP} "killall LaundryKiosk 2>/dev/null; sleep 1; cd ${PI_DEST} && DISPLAY=:0 ./build/LaundryKiosk &"

echo "=== Done! App đang chạy trên Pi ==="
