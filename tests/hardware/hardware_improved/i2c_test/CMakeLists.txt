cmake_minimum_required(VERSION 3.16)
project(i2c_hardware_test CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DRIVER_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../src/drivers" CACHE PATH "Path to driver root directory")
if (NOT EXISTS "${DRIVER_ROOT}/i2c/i2c.hpp")
    set(DRIVER_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../drivers" CACHE PATH "Path to driver root directory" FORCE)
endif()

add_executable(i2c_hardware_test
    main.cpp
    ${DRIVER_ROOT}/i2c/i2c.cpp
)

target_include_directories(i2c_hardware_test PRIVATE
    ${DRIVER_ROOT}/i2c
)

target_compile_options(i2c_hardware_test PRIVATE -Wall -Wextra -Wpedantic)
