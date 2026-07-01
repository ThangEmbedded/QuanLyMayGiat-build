cmake_minimum_required(VERSION 3.10)

project(ads1115_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ads1115_test
    ads1115_test.cpp
    ../../../src/drivers/ads1115/ads1115.cpp
    ../../../src/drivers/i2c/i2c.cpp
)

target_include_directories(ads1115_test PRIVATE
    ../../../src/drivers/i2c
    ../../../src/drivers/ads1115
)