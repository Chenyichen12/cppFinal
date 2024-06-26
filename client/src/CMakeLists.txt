cmake_minimum_required(VERSION 3.5)
enable_testing()
project(plant VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(.)

find_package(Qt6 REQUIRED COMPONENTS Gui Widgets Svg Core websockets)

find_package(Eigen3 CONFIG REQUIRED)


file(GLOB_RECURSE PROJECT_SOURCES
        ../src/*.cpp
        ../src/*.h
        ../src/*.hpp
)

link_libraries(Eigen3::Eigen)

function(add_main_executable)

    set(MAIN_SOURCE ${PROJECT_SOURCES})
    file(GLOB_RECURSE TEST_FILES ../src/test/*)
    foreach (TEST_FILE ${TEST_FILES})
        list(REMOVE_ITEM MAIN_SOURCE ${TEST_FILE})
    endforeach ()

    qt_add_executable(PlantTree
            MANUAL_FINALIZATION
            ${MAIN_SOURCE}
            ../res/svg.qrc
            ../res/mat.qrc
    )

    target_precompile_headers(PlantTree PRIVATE <QCoreApplication>
            <QObject>
            <QWidget>
            <QList>
            <QString>
            <QVector>
            <QMap>)
    target_link_libraries(PlantTree PRIVATE Qt6::Widgets
            PRIVATE Qt6::Svg PRIVATE Qt6::Gui PRIVATE Qt6::Core
            PRIVATE Qt6::WebSockets
    )

endfunction(add_main_executable)

add_main_executable()

function(add_test_exectable)
    find_package(GTest CONFIG REQUIRED)
    qt_add_executable(treeTest
            ${PROJECT_SOURCES}
            ../res/svg.qrc
            ../res/mat.qrc
    )
    target_compile_definitions(treeTest PRIVATE TREE_TEST)

    target_link_libraries(treeTest PRIVATE Qt6::Widgets
            PRIVATE Qt6::Svg PRIVATE Qt6::Gui PRIVATE Qt6::Core
            PRIVATE Qt6::WebSockets
            PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
    )
endfunction(add_test_exectable)

add_test_exectable()

