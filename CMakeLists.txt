include(${CMAKE_CURRENT_LIST_DIR}/QtProjectSetup.cmake OPTIONAL)

cmake_minimum_required(VERSION 3.16)

project(ForbiddenIsland VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt with required modules
find_package(Qt6 REQUIRED COMPONENTS Widgets)

message(STATUS "Qt6Widgets include dirs: ${Qt6Widgets_INCLUDE_DIRS}")
message(STATUS "Qt6Widgets libraries: ${Qt6Widgets_LIBRARIES}")


# Recommended Qt setup macro — handles AUTOMOC, AUTOUIC, AUTORCC
qt_standard_project_setup()

include_directories(${Qt6Widgets_INCLUDE_DIRS})


# Use modern executable setup that registers with Qt tools properly
qt_add_executable(ForbiddenIsland
    Game.cpp
    Main.cpp
    Deck.cpp
    Card.cpp
    Hand.cpp
    Player.cpp
    ForbiddenIslandUI.cpp
)

# Link Qt libraries
target_link_libraries(ForbiddenIsland PRIVATE Qt6::Widgets)