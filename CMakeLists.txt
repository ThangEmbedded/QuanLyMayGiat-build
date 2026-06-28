cmake_minimum_required(VERSION 3.16)

project(QuanLyMayGiat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Include path
# =========================
include_directories(
    src
)

# =========================
# libgpiod (Pi + Host nếu cài)
# =========================
find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED libgpiodcxx)

# =========================
# GPIO driver
# =========================
add_library(gpio_driver
    src/drivers/gpio/gpio.cpp
)

target_include_directories(gpio_driver PUBLIC
    src/drivers/gpio
)

target_link_libraries(gpio_driver
    ${GPIOD_LIBRARIES}
)

# =========================
# Relay driver
# =========================
add_library(relay_driver
    src/drivers/relay/relay.cpp
)

target_include_directories(relay_driver PUBLIC
    src/drivers/relay
)

target_link_libraries(relay_driver
    gpio_driver
)

# =========================
# Relay hardware test
# =========================
add_executable(relay_test
    tests/hardware/relay_test.cpp
)

target_link_libraries(relay_test
    relay_driver
)

# =========================
# output folder
# =========================
set_target_properties(relay_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)