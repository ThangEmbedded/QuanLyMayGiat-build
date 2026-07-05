cmake_minimum_required(VERSION 3.16)
project(LaundryKiosk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

option(LAUNDRY_USE_REAL_RELAY "Use Raspberry Pi relay hardware driver" ON)

get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(LAUNDRY_UI_SOURCES
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
    services/RelayHardwareService.cpp
    services/RelayHardwareService.h

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

add_executable(laundry_kiosk ${LAUNDRY_UI_SOURCES})

target_include_directories(laundry_kiosk PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_ROOT}/src
    ${PROJECT_ROOT}/src/drivers/gpio
    ${PROJECT_ROOT}/src/drivers/relay
)

target_link_libraries(laundry_kiosk PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(LAUNDRY_USE_REAL_RELAY)
    target_compile_definitions(laundry_kiosk PRIVATE LAUNDRY_USE_REAL_RELAY=1)

    if(TARGET relay_driver)
        target_link_libraries(laundry_kiosk PRIVATE relay_driver)
    else()
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(GPIOD REQUIRED IMPORTED_TARGET libgpiod)
        target_sources(laundry_kiosk PRIVATE
            ${PROJECT_ROOT}/src/drivers/gpio/gpio.cpp
            ${PROJECT_ROOT}/src/drivers/relay/relay.cpp
        )
        target_link_libraries(laundry_kiosk PRIVATE PkgConfig::GPIOD)
    endif()
endif()

set_target_properties(laundry_kiosk PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
