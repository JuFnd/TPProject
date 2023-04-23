cmake_minimum_required(VERSION 3.14)

project(SessionFeature LANGUAGES CXX)

include_directories(header)
include_directories(src)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
## Подключаем Gstreamer
#find_package(PkgConfig) #finding pkg-config is a helper tool

##using pkg-config to getting Gstreamer
#pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

##including GStreamer header files directory
#include_directories(
#        ${CMAKE_CURRENT_LIST_DIR}/include
#        ${GLIB_INCLUDE_DIRS}
#        ${GSTREAMER_INCLUDE_DIRS}
#)

##linking GStreamer library directory
#link_directories(
#        ${GLIB_LIBRARY_DIRS}
#        ${GSTREAMER_LIBRARY_DIRS}
#)

#приемник --------------------------------------
add_library(reciver
            STATIC
            header/reciver.h
            src/reciver.c
            )

target_link_libraries(reciver
                        Qt${QT_VERSION_MAJOR}::Core
                        ${GSTREAMER_LIBRARIES}
                        )
#------------------------------------------------


# Фигня из примера
add_library(mytread
            STATIC
            header/mythread.h
            src/mythread.cpp
            )

target_link_libraries(mytread
                        Qt${QT_VERSION_MAJOR}::Core
                        )

#------------------------

add_executable(SessionFeature
  main.cpp
)
target_link_libraries(SessionFeature
                        Qt${QT_VERSION_MAJOR}::Core
#                        mytread
                        reciver
                    )


install(TARGETS SessionFeature
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})





