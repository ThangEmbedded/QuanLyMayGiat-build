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
