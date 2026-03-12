#include "../include/GameEngine.h"
#include <random>
#include "ResourceOrder.h"

std::unique_ptr<Order> GameEngine::generateRandomOrder() {
    std::uniform_int_distribution<int> resourceDist(0, 4);
    std::uniform_int_distribution<int> amountDist(1, 5);
    Resource::Type type = static_cast<Resource::Type>(resourceDist(rng_));
    int amount = amountDist(rng_);
    int reward = amount * 20 + 10;

    return std::make_unique<ResourceOrder>(type, amount, reward);
}


GameEngine::GameEngine(){
    keepRunning=true;
    ships_.emplace_back(100.0f, 50.0f, 2.0f);
    ships_.emplace_back(100.0f, 150.0f, 1.5f);
    ships_.emplace_back(100.0f, 250.0f, 2.5f);
    ships_.emplace_back(100.0f, 200.0f, 2.0f);
    map = {
        "--------------------------------",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|  |  |   |  |   |  |   |  |   |",
        "|--|  |---|  |---|  |---|  |---|",
        "|   __     __     __     __    |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "|                              |",
        "--------------------------------"
    };
}

 void GameEngine::movements(InformationPlayer& player) {
    float speed = 0.1f;
    float oldX = player.getX();
    float oldY = player.getY();
    float nextX = oldX;
    float nextY = oldY;
    float s = 0.85f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) nextY -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) nextY += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) nextX -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) nextX += speed;

    if (map[(int)oldY][(int)nextX] == ' ' && map[(int)(oldY + s)][(int)nextX] == ' ' &&
        map[(int)oldY][(int)(nextX + s)] == ' ' && map[(int)(oldY + s)][(int)(nextX + s)] == ' ') {
        player.SetPosition(nextX, oldY);
    }

    float currentX = player.getX();
    if (map[(int)nextY][(int)currentX] == ' ' && map[(int)(nextY + s)][(int)currentX] == ' ' &&
        map[(int)nextY][(int)(currentX + s)] == ' ' && map[(int)(nextY + s)][(int)(currentX + s)] == ' ') {
        player.SetPosition(currentX, nextY);
    }
}

void GameEngine::run(InformationPlayer& player) {
    sf::RenderWindow window(sf::VideoMode({WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE}), "SpaceGame");
    window.setFramerateLimit(60);
    std::uniform_int_distribution<int> chanceDist(0, 500);
    std::vector<float> Gates = { 4.5f * 28.0f, 11.5f * 28.0f, 18.5f * 28.0f, 25.5f * 28.0f };

    while (window.isOpen() && keepRunning) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        movements(player);
        for (size_t i = 0; i < ships_.size(); ++i) {
            auto& ship = ships_[i];
            if (ship.isActive()&& !ship.hasOrder()) {
                if (chanceDist(rng_) == 0) ship.setOrder(generateRandomOrder());
            }
            float station = 0.0f;
            if (ship.hasOrder() && i < Gates.size()) {
                station = Gates[i];
            }

            ship.update(station);
        }
        window.clear(sf::Color::Black);
        Map::draw(window, map, player, ships_);
        window.display();
    }
}




