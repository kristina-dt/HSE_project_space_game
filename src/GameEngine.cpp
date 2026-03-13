#include "../include/GameEngine.h"
#include <random>
#include "Foodmaker.h"
#include "ResourceOrder.h"
#include <optional>
#include <StartScreen.h>

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

void GameEngine::run(InformationPlayer& player, Map& mapp) {
    float cellSize = 28.0f;
    sf::RenderWindow window(sf::VideoMode({WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE}),"SpaceGame");
    window.setFramerateLimit(60);

    sf::Font font;
    font.openFromFile("fonts/DeluxePaint-Regular.otf");
    StartScreen startScreen(font);
    startScreen.setPosition({8.5*28.0f, 15*28.0f});

    bool isGameStarted = false;

    std::uniform_int_distribution<int> chanceDist(0, 500);
    std::vector<float> Gates = {4.5f * 28.0f, 11.5f * 28.0f, 18.5f * 28.0f, 25.5f * 28.0f};

    sf::FloatRect foodmakerZone({1.0f * cellSize, 18.0f * cellSize},{2 * cellSize, 4 * cellSize});
    sf::FloatRect fuelmakerZone({29.0f * cellSize,16.0f * cellSize},{2 * cellSize, 4 * cellSize});
    sf::FloatRect partassemblerZone({22.0f * cellSize,22.0f * cellSize},{2 * cellSize, 4 * cellSize});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (!isGameStarted) {
                startScreen.typed(*event);
                if (const auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
                    if (textEvent->unicode == 32 && !startScreen.getText().empty()) {
                        isGameStarted = true;
                    }
                }
            }
        }

        window.clear(sf::Color::Black);
        if (!isGameStarted) {
            startScreen.drawTo(window);
        }
        else {
            movements(player);
            sf::FloatRect playerBounds({player.getX() * cellSize, player.getY() * cellSize},{cellSize, cellSize});
            for (size_t i = 0; i < ships_.size(); ++i) {
                auto& ship = ships_[i];
                if (ship.isActive() && !ship.hasOrder()) {
                    if (chanceDist(rng_) == 0)
                        ship.setOrder(generateRandomOrder());
                }
                float station = 0.f;
                if (ship.hasOrder() && i < Gates.size()) {
                    station = Gates[i];
                }
                ship.update(station);
            }

            Map::draw(window, map, player, ships_);

            if (playerBounds.findIntersection(foodmakerZone)) {
                mapp.prepareWindow(WindowType::Food);
                mapp.drawWindow(window);
            }
            if (playerBounds.findIntersection(fuelmakerZone)) {
                mapp.prepareWindow(WindowType::Fuel);
                mapp.drawWindow(window);
            }
            if (playerBounds.findIntersection(partassemblerZone)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
                    mapp.updateAmount(WindowType::PartAssembler);
                }
                mapp.prepareWindow(WindowType::PartAssembler);
                mapp.drawWindow(window);
            }

        }

        window.display();
    }
}






