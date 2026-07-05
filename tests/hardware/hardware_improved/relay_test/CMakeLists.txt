cmake_minimum_required(VERSION 3.16)
project(relay_hardware_test CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DRIVER_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../src/drivers" CACHE PATH "Path to driver root directory")
if (NOT EXISTS "${DRIVER_ROOT}/relay/relay.hpp")
    set(DRIVER_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../drivers" CACHE PATH "Path to driver root directory" FORCE)
endif()

add_executable(relay_hardware_test
    main.cpp
    ${DRIVER_ROOT}/relay/relay.cpp
    ${DRIVER_ROOT}/gpio/gpio.cpp
)

target_include_directories(relay_hardware_test PRIVATE
    ${DRIVER_ROOT}/relay
    ${DRIVER_ROOT}/gpio
)

find_library(GPIOD_LIB gpiod REQUIRED)
target_link_libraries(relay_hardware_test PRIVATE ${GPIOD_LIB})
target_compile_options(relay_hardware_test PRIVATE -Wall -Wextra -Wpedantic)
