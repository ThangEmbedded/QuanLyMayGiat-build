cmake_minimum_required(VERSION 3.14)
project(LaundryKiosk)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(LaundryKiosk
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/pages/HomePage.h
    src/pages/MachineDetailPage.h
    src/pages/PaymentPage.h
    src/pages/RunningStatusPage.h
    src/pages/AdminLoginPage.h
    src/pages/AdminDashboardPage.h
)

target_link_libraries(LaundryKiosk Qt6::Widgets)
