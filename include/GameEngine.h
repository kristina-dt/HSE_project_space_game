#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include <iostream>

class GameEngine {
    std::vector<std::string> map;
    bool keepRunning;
    const unsigned int WIDTH = 30;
    const unsigned int  HEIGHT = 16;
    const unsigned int  CELL_SIZE = 25;
public:
    GameEngine();
    void run(InformationPlayer& player);
    void movements(InformationPlayer& player);
};

