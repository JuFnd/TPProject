cmake_minimum_required(VERSION 3.5)

project(bt7 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 COMPONENTS Bluetooth REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Multimedia REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(KF5BluezQt REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        client.h
        client.cpp
        server.cpp
        server.h
        remoteselector.cpp
        remoteselector.h
        remoteselector.ui
        bluetoothformatimage.cpp
        bluetoothformatimage.h
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(bt7
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET bt7 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(bt7 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(bt7
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(bt7 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(bt7 PRIVATE Qt5::Bluetooth)
target_link_libraries(bt7 PRIVATE Qt5::Core)
target_link_libraries(bt7 PRIVATE Qt5::Multimedia)
target_link_libraries(bt7 PRIVATE KF5::BluezQt)
target_link_libraries(bt7 PRIVATE Qt5::DBus)

set_target_properties(bt7 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS bt7
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(bt7)
endif()
