cmake_minimum_required(VERSION 3.10)

# Define project name
project(ads1115_test)

# Add executable
add_executable(ads1115_test ../src/drivers/ads1115/ads1115_test.cpp ../src/drivers/ads1115/ads1115.cpp)

# Link the necessary libraries
# Adjust this to link against your actual I2C implementation if needed
target_link_libraries(ads1115_test PRIVATE your_i2c_library)  

include_directories(${CMAKE_CURRENT_SOURCE_DIR})