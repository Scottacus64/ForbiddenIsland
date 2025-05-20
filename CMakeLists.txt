cmake_minimum_required(VERSION 3.5)

project(ForbiddenIsland VERSION 1.0 LANGUAGES CXX)

# Enable debug symbols
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt5 automatic tools (if you use Qt; otherwise remove these)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Add the executable
add_executable(ForbiddenIsland
    ${CMAKE_SOURCE_DIR}/Game.cpp
    ${CMAKE_SOURCE_DIR}/Main.cpp
    ${CMAKE_SOURCE_DIR}/Deck.cpp
    ${CMAKE_SOURCE_DIR}/Card.cpp
    ${CMAKE_SOURCE_DIR}/Hand.cpp
    ${CMAKE_SOURCE_DIR}/Player.cpp
)