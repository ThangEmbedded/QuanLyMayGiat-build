cmake_minimum_required(VERSION 3.16)
project(LaundryKioskDemo3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

option(USE_REAL_RELAY "Use real relay driver from project src/drivers/relay" ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Layout mặc định khi đặt folder này tại: QuanLyMayGiat/tests/demo_3/ui
get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)

set(APP_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
    resources.qrc

    models/MachineData.h
    models/MachineState.h
    models/WashCycle.h

    controllers/MachineController.cpp
    controllers/MachineController.h

    services/IHardwareService.h
    services/MockHardwareService.cpp
    services/MockHardwareService.h
    services/IRelayService.h
    services/MockRelayService.cpp
    services/MockRelayService.h

    pages/AdminDashboardPage.cpp
    pages/AdminDashboardPage.h
    pages/AdminAuthPage.cpp
    pages/AdminAuthPage.h
    pages/HomePage.cpp
    pages/HomePage.h
    pages/MachineDetailPage.cpp
    pages/MachineDetailPage.h
    pages/ConfirmStartPage.cpp
    pages/ConfirmStartPage.h

    widgets/CircularProgressRing.cpp
    widgets/CircularProgressRing.h
    widgets/HeaderWidget.cpp
    widgets/HeaderWidget.h
    widgets/MachineCardWidget.cpp
    widgets/MachineCardWidget.h
    widgets/TransitionStackedWidget.cpp
    widgets/TransitionStackedWidget.h
    widgets/NumericInputDialog.cpp
    widgets/NumericInputDialog.h
)

if(USE_REAL_RELAY)
    list(APPEND APP_SOURCES
        services/RealRelayService.cpp
        services/RealRelayService.h
        ${PROJECT_ROOT}/src/drivers/gpio/gpio.cpp
        ${PROJECT_ROOT}/src/drivers/relay/relay.cpp
    )
endif()

add_executable(laundry_kiosk_demo_3 ${APP_SOURCES})

target_include_directories(laundry_kiosk_demo_3 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_ROOT}/src
)

if(USE_REAL_RELAY)
    target_compile_definitions(laundry_kiosk_demo_3 PRIVATE USE_REAL_RELAY=1)
endif()

target_link_libraries(laundry_kiosk_demo_3 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
