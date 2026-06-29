cmake_minimum_required(VERSION 3.16)

project(i2c_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(i2c_test
    main.cpp
)

target_sources(i2c_test PRIVATE
    ../../../src/drivers/i2c/i2c.cpp
)

target_include_directories(i2c_test PRIVATE
    ../../../src/drivers/i2c
)

target_compile_options(i2c_test PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)