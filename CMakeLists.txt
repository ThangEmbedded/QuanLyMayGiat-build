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
pkg_check_modules(GPIOD REQUIRED libgpiod)

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
# I2C driver
# =========================
add_library(i2c_driver
    src/drivers/i2c/i2c.cpp
)

target_include_directories(i2c_driver PUBLIC
    src/drivers/i2c
)

# =========================
# ADS1115 driver
# =========================
add_library(ads1115_driver
    src/drivers/ads1115/ads1115.cpp
)

target_include_directories(ads1115_driver PUBLIC
    src/drivers/ads1115
)

# =========================
# I2C hardware test
# =========================
add_executable(i2c_test
    src/tests/hardware/i2c_test.cpp
)

target_link_libraries(i2c_test
    i2c_driver
)

# =========================
# ADS1115 hardware test
# =========================
add_executable(ads1115_test
    src/tests/hardware/ads1115_test.cpp
)

target_link_libraries(ads1115_test
    i2c_driver
    ads1115_driver
)

# =========================
# output folder
# =========================
set_target_properties(i2c_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

set_target_properties(ads1115_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

# =========================
# Relay hardware test
# =========================
add_executable(relay_test
    tests/hardware/relay_test/main.cpp
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
# =========================
# Main Qt kiosk application
# =========================
add_subdirectory(src/ui)
