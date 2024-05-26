cmake_minimum_required(VERSION 3.5)
enable_testing()
project(plant_serve VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(.)

find_package(Qt6 REQUIRED COMPONENTS Gui Widgets Svg Core Network websockets)
find_package(Eigen3 REQUIRED)
file(GLOB_RECURSE PROJECT_SOURCES
        ../src/*.cpp
        ../src/*.h
        ../src/*.hpp
)

qt_add_executable(plant_serve
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
)
target_link_libraries(plant_serve PRIVATE Qt6::Widgets
        PRIVATE Qt6::Gui PRIVATE Qt6::Core PRIVATE Qt6::Network PRIVATE Qt6::WebSockets
        Eigen3::Eigen
)