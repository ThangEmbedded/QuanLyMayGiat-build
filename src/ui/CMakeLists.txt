cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(laundry_kiosk
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
    services/ICurrentSensorService.h
    services/MockCurrentSensorService.h
    services/Ads1115CurrentSensorService.cpp
    services/Ads1115CurrentSensorService.h

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

target_include_directories(laundry_kiosk PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(laundry_kiosk PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    relay_driver
    ads1115_driver
    i2c_driver
)
