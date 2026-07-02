cmake_minimum_required(VERSION 3.16)
project(ads1115_hardware_test CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DRIVER_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../src/drivers" CACHE PATH "Path to driver root directory")
if (NOT EXISTS "${DRIVER_ROOT}/ads1115/ads1115.hpp")
    set(DRIVER_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../drivers" CACHE PATH "Path to driver root directory" FORCE)
endif()

add_executable(ads1115_hardware_test
    ads1115_test.cpp
    ${DRIVER_ROOT}/ads1115/ads1115.cpp
    ${DRIVER_ROOT}/i2c/i2c.cpp
)

target_include_directories(ads1115_hardware_test PRIVATE
    ${DRIVER_ROOT}/ads1115
    ${DRIVER_ROOT}/i2c
)

target_compile_options(ads1115_hardware_test PRIVATE -Wall -Wextra -Wpedantic)
