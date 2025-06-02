cmake_minimum_required(VERSION 3.16)

project(ForbiddenIsland VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Setup Qt project (AUTOMOC, AUTOUIC, etc.)
qt_standard_project_setup()

# Add app target
qt_add_executable(ForbiddenIsland
    main.cpp
    Game.cpp
    ForbiddenIslandUI.cpp
    Card.cpp
    Deck.cpp
    Hand.cpp
    Player.cpp
)

# Resource file (if using .qrc)
#qt_add_resources(APP_RESOURCES resources.qrc)
#target_sources(ForbiddenIsland PRIVATE ${APP_RESOURCES})

# Link Qt libraries
target_link_libraries(ForbiddenIsland PRIVATE Qt6::Widgets)