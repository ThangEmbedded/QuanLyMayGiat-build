cmake_minimum_required(VERSION 3.14)
project(LaundryKiosk)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable AUTOUIC, AUTOMOC, and AUTORCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Try Qt6 first, fallback to Qt5
find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    message(STATUS "Found Qt6 Widgets")
    set(QT_LIBRARIES Qt6::Widgets)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    message(STATUS "Found Qt5 Widgets")
    set(QT_LIBRARIES Qt5::Widgets)
endif()

add_executable(LaundryKiosk
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    
    src/widgets/TransitionStackedWidget.h
    src/widgets/TransitionStackedWidget.cpp
    src/widgets/CircularProgressRing.h
    src/widgets/CircularProgressRing.cpp
    src/widgets/QRWidget.h
    src/widgets/QRWidget.cpp
    src/widgets/HeaderWidget.h
    src/widgets/HeaderWidget.cpp
    src/widgets/MachineCardWidget.h
    src/widgets/MachineCardWidget.cpp
    
    src/pages/HomePage.h
    src/pages/HomePage.cpp
    src/pages/MachineDetailPage.h
    src/pages/MachineDetailPage.cpp
    src/pages/PaymentPage.h
    src/pages/PaymentPage.cpp
    src/pages/RunningStatusPage.h
    src/pages/RunningStatusPage.cpp
    src/pages/AdminLoginPage.h
    src/pages/AdminLoginPage.cpp
    src/pages/AdminDashboardPage.h
    src/pages/AdminDashboardPage.cpp
    
    src/resources.qrc
)

include_directories(src)

target_link_libraries(LaundryKiosk ${QT_LIBRARIES})
