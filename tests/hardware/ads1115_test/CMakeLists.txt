cmake_minimum_required(VERSION 3.10)
project(ads1115_test)

set(CMAKE_CXX_STANDARD 17)

add_executable(ads1115_test main.cpp)

target_include_directories(ads1115_test PRIVATE
    ../../src/drivers/ads1115
    ../../src/drivers/i2c
)

# Link any necessary libraries
# target_link_libraries(ads1115_test ...)
