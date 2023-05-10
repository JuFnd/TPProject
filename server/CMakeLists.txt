cmake_minimum_required(VERSION 3.14)

project(server LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Core
    Network
    Widgets
    Sql
)


file(GLOB PROJECT_SRC
    ${CMAKE_CURRENT_LIST_DIR}/sources/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include/*.h
)

add_executable(${PROJECT_NAME}
    ${PROJECT_SRC}
)


target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Widgets
         Qt${QT_VERSION_MAJOR}::Sql
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

install(TARGETS server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
