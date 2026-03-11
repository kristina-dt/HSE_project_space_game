#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include <iostream>

class GameEngine {
    std::vector<std::string> map;
    bool keepRunning;
    const unsigned int WIDTH = 30;
    const unsigned int  HEIGHT = 25;
public:
    static const unsigned int  CELL_SIZE = 32;
    GameEngine();
    void run(InformationPlayer& player);
    void movements(InformationPlayer& player);
};

