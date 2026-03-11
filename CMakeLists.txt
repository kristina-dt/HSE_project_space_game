cmake_minimum_required(VERSION 3.20)
project(HSE_project_space_game)
set(CMAKE_CXX_STANDARD 23)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_subdirectory(src)
include_directories(include)
add_executable(SpaceGame main.cpp
        src/Map.cpp
        include/Map.h
        src/NotEnoughMoney.cpp
        include/NotEnoughMoney.h
        src/wallet.cpp
        include/wallet.h
        src/InformationPlayer.cpp
        include/InformationPlayer.h
        src/GameEngine.cpp
        include/GameEngine.h
        src/ProductionManager.cpp
        include/ProductionManager.h
        src/Ship.cpp
        include/Ship.h
        src/ResourceOrder.cpp
        include/ResourceOrder.h
        src/PartAssembler.cpp
        include/PartAssembler.h
        src/Order.cpp
        include/Order.h
        src/FuelMaker.cpp
        include/FuelMaker.h
        src/FoodMaker.cpp
        include/Foodmaker.h
        src/Appliance.cpp
        include/Appliance.h

)

target_include_directories(SpaceGame PUBLIC include)
target_link_libraries(SpaceGame SFML::Graphics SFML::Window SFML::System)