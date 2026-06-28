cmake_minimum_required(VERSION 3.10)
project(RelayHardwareTest CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Đóng gói nguồn bao gồm cả tầng test, tầng xử lý Relay và tầng Driver GPIO bên dưới
set(TEST_SOURCES
    main.cpp
    ../../../src/drivers/relay/relay.cpp  # Tầng xử lý logic Relay của bạn
    ../../../src/drivers/gpio/gpio.cpp    # Tầng Driver GPIO điều khiển libgpiod
)

add_executable(hardware_relay_test ${TEST_SOURCES})

# Mở các đường dẫn include để main.cpp tìm thấy relay.hpp và relay.cpp tìm thấy gpio.hpp
target_include_directories(hardware_relay_test PRIVATE 
    ../../../src/drivers/relay
    ../../../src/drivers/gpio
)

# Liên kết với thư viện hệ thống
find_library(GPIOD_LIB gpiod)
target_link_libraries(hardware_relay_test PRIVATE ${GPIOD_LIB})