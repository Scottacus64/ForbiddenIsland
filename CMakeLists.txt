# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.5)

# Set the project name and version
project(ForbiddenIsland VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(ForbiddenIsland
    ${CMAKE_SOURCE_DIR}/Game.cpp
    ${CMAKE_SOURCE_DIR}/Main.cpp
    ${CMAKE_SOURCE_DIR}/Deck.cpp
    ${CMAKE_SOURCE_DIR}/Card.cpp
    ${CMAKE_SOURCE_DIR}/Hand.cpp
    ${CMAKE_SOURCE_DIR}/Player.cpp
)
cmake_minimum_required(VERSION 3.5)

project(ForbiddenIsland VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

