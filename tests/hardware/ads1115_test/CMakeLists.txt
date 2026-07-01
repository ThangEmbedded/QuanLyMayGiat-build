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
     /home/toppi/work_data/1_projects/QuanLyMayGiat/src/drivers/ads1115
     /home/toppi/work_data/1_projects/QuanLyMayGiat/src/drivers/i2c
)
