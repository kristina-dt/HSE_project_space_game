#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include <iostream>
#include <random>
#include "Order.h"

class GameEngine {
    std::vector<std::string> map;
    bool keepRunning;
    const unsigned int WIDTH = 30;
    const unsigned int  HEIGHT = 25;
    std::vector<Ship> ships_;
    std::mt19937 rng_{ std::random_device{}() };
    std::unique_ptr<Order> generateRandomOrder();
public:
    static const unsigned int  CELL_SIZE = 30;
    GameEngine();
    void run(InformationPlayer& player);
    void movements(InformationPlayer& player);
};

