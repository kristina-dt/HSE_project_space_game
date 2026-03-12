#include "../include/GameEngine.h"

GameEngine::GameEngine(){
    keepRunning=true;
    ships_.emplace_back(140.0f, 100.0f, 2.0f);
    ships_.emplace_back(100.0f, 196.0f, 1.5f);
    ships_.emplace_back(196.0f, 252.0f, 3.0f);
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
        "--------------------------------"
    };
}

 void GameEngine::movements(InformationPlayer& player) {
    float speed = 0.1f;
    float oldX = player.getX();
    float oldY = player.getY();
    float nextX = oldX;
    float nextY = oldY;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) nextY -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) nextY += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) nextX -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) nextX += speed;

    if (map[(int)oldY][(int)nextX] == ' ') {
        player.SetPosition(nextX, oldY);
    }
    float currentX = player.getX();
    if (map[(int)nextY][(int)currentX] == ' ') {
        player.SetPosition(currentX, nextY);
    }

}

void GameEngine::run(InformationPlayer& player) {
    sf::RenderWindow window(sf::VideoMode({WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE}), "SpaceGame");
    window.setFramerateLimit(60);

    while (window.isOpen() && keepRunning) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        movements(player);
        const float station = 11 * 28.0f;
        for (auto& ship : ships_) {
            ship.update(station);
        }
        window.clear(sf::Color::Black);
        Map::draw(window, map, player, ships_);
        window.display();
    }
}




