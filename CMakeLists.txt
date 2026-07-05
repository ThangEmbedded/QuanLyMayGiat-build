cmake_minimum_required(VERSION 3.16)

project(QuanLyMayGiat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_HARDWARE_TESTS "Build hardware test binaries" OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED IMPORTED_TARGET libgpiod)

add_library(gpio_driver
    src/drivers/gpio/gpio.cpp
)
target_include_directories(gpio_driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/gpio
)
target_link_libraries(gpio_driver PUBLIC PkgConfig::GPIOD)

add_library(relay_driver
    src/drivers/relay/relay.cpp
)
target_include_directories(relay_driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/relay
)
target_link_libraries(relay_driver PUBLIC gpio_driver)

add_library(i2c_driver
    src/drivers/i2c/i2c.cpp
)
target_include_directories(i2c_driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/i2c
)

add_library(ads1115_driver
    src/drivers/ads1115/ads1115.cpp
)
target_include_directories(ads1115_driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/ads1115
)

add_subdirectory(src/ui)

if(BUILD_HARDWARE_TESTS)
    add_executable(relay_test tests/hardware/relay_test/main.cpp)
    target_link_libraries(relay_test PRIVATE relay_driver)

    add_executable(i2c_test src/tests/hardware/i2c_test.cpp)
    target_link_libraries(i2c_test PRIVATE i2c_driver)

    add_executable(ads1115_test src/tests/hardware/ads1115_test.cpp)
    target_link_libraries(ads1115_test PRIVATE i2c_driver ads1115_driver)

    set_target_properties(relay_test i2c_test ads1115_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )
endif()
