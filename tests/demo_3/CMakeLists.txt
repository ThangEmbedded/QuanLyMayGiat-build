cmake_minimum_required(VERSION 3.16)
project(LaundryDemo3RealRelay LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(UI_DIR "${PROJECT_ROOT}/src/ui")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED IMPORTED_TARGET libgpiod)

add_executable(laundry_kiosk_demo_3
    main.cpp

    ${UI_DIR}/MainWindow.cpp
    ${UI_DIR}/MainWindow.h
    ${UI_DIR}/resources.qrc

    ${UI_DIR}/models/MachineData.h
    ${UI_DIR}/models/MachineState.h
    ${UI_DIR}/models/WashCycle.h

    ${UI_DIR}/controllers/MachineController.cpp
    ${UI_DIR}/controllers/MachineController.h

    ${UI_DIR}/services/IHardwareService.h
    ${UI_DIR}/services/MockHardwareService.cpp
    ${UI_DIR}/services/MockHardwareService.h
    ${UI_DIR}/services/IRelayService.h
    ${UI_DIR}/services/MockRelayService.cpp
    ${UI_DIR}/services/MockRelayService.h
    ${UI_DIR}/services/RelayHardwareService.cpp
    ${UI_DIR}/services/RelayHardwareService.h

    ${UI_DIR}/pages/AdminDashboardPage.cpp
    ${UI_DIR}/pages/AdminDashboardPage.h
    ${UI_DIR}/pages/AdminAuthPage.cpp
    ${UI_DIR}/pages/AdminAuthPage.h
    ${UI_DIR}/pages/HomePage.cpp
    ${UI_DIR}/pages/HomePage.h
    ${UI_DIR}/pages/MachineDetailPage.cpp
    ${UI_DIR}/pages/MachineDetailPage.h
    ${UI_DIR}/pages/ConfirmStartPage.cpp
    ${UI_DIR}/pages/ConfirmStartPage.h

    ${UI_DIR}/widgets/CircularProgressRing.cpp
    ${UI_DIR}/widgets/CircularProgressRing.h
    ${UI_DIR}/widgets/HeaderWidget.cpp
    ${UI_DIR}/widgets/HeaderWidget.h
    ${UI_DIR}/widgets/MachineCardWidget.cpp
    ${UI_DIR}/widgets/MachineCardWidget.h
    ${UI_DIR}/widgets/TransitionStackedWidget.cpp
    ${UI_DIR}/widgets/TransitionStackedWidget.h
    ${UI_DIR}/widgets/NumericInputDialog.cpp
    ${UI_DIR}/widgets/NumericInputDialog.h

    ${PROJECT_ROOT}/src/drivers/gpio/gpio.cpp
    ${PROJECT_ROOT}/src/drivers/gpio/gpio.hpp
    ${PROJECT_ROOT}/src/drivers/gpio/gpio_types.hpp
    ${PROJECT_ROOT}/src/drivers/relay/relay.cpp
    ${PROJECT_ROOT}/src/drivers/relay/relay.hpp
    ${PROJECT_ROOT}/src/drivers/relay/relay_config.hpp
)

target_include_directories(laundry_kiosk_demo_3 PRIVATE
    ${PROJECT_ROOT}/src
    ${UI_DIR}
    ${PROJECT_ROOT}/src/drivers/gpio
    ${PROJECT_ROOT}/src/drivers/relay
)

target_link_libraries(laundry_kiosk_demo_3 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    PkgConfig::GPIOD
)
