#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"

class GameEngine {
    std::vector<std::string> map;
    bool keepRunning;
    const int WIDTH = 30;
    const int HEIGHT = 13;
    const float CELL_SIZE = 32.0f;
public:
    GameEngine();
    void run(InformationPlayer& player);
    void movements(InformationPlayer& player);
};

