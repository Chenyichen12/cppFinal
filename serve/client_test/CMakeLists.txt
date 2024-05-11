cmake_minimum_required(VERSION 3.5)
enable_testing()
project(plant_test_serve VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(.)

find_package(Qt6 REQUIRED COMPONENTS Gui Widgets Svg Core Network websockets)

file(GLOB_RECURSE PROJECT_SOURCES
        ../client_test/*.cpp
        ../client_test/*.h
        ../client_test/*.hpp
)

qt_add_executable(plant_test_serve
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
)
target_compile_definitions(plant_test_serve PRIVATE json_test="${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(plant_test_serve PRIVATE Qt6::Widgets
        PRIVATE Qt6::Gui PRIVATE Qt6::Core PRIVATE Qt6::Network PRIVATE Qt6::WebSockets
)