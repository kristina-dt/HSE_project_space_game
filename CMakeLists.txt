cmake_minimum_required(VERSION 3.20)
project(HSE_project_space_game)
set(CMAKE_CXX_STANDARD 23)

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
        include/InformationPlayer.h)
        src/GameEngine.cpp
        include/GameEngine.h)
