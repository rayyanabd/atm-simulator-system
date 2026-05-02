cmake_minimum_required(VERSION 3.16)

project(ATMSimulatorSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(BACKEND_SOURCES
    src/Account.cpp
    src/ATM.cpp
    src/CurrentAccount.cpp
    src/Transaction.cpp
)

add_library(atm_backend ${BACKEND_SOURCES})
target_include_directories(atm_backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(atm_console src/main.cpp)
target_link_libraries(atm_console PRIVATE atm_backend)

add_executable(atm_qt
    src/qt_main.cpp
    gui/MainWindow.cpp
    gui/MainWindow.h
)

target_include_directories(atm_qt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(atm_qt PRIVATE atm_backend Qt${QT_VERSION_MAJOR}::Widgets)
