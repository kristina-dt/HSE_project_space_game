#include "../include/Map.h"
#include "GameEngine.h"
#include <iostream>

void Map::draw(sf::RenderWindow &window, const std::vector<std::string> &map, const InformationPlayer &player, const std::vector<Ship>& ships) {
    const float cellSize = 28.0f;
    sf::RectangleShape tile(sf::Vector2f(cellSize, cellSize));

    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            tile.setPosition({x * cellSize, y * cellSize});

            if (map[y][x] == '-' || map[y][x] == '|' || map[y][x] == '_' )
                tile.setFillColor(sf::Color(100, 100, 100));
            else
                tile.setFillColor(sf::Color::Black);
            window.draw(tile);
        }
    }

    sf::RectangleShape fmakerShape(sf::Vector2f(cellSize*2,cellSize*4));
    fmakerShape.setFillColor(sf::Color::Blue);
    fmakerShape.setPosition({1.0f * cellSize,18.0f * cellSize});
    window.draw(fmakerShape);

    sf::RectangleShape flmakerShape(sf::Vector2f(cellSize*2,cellSize*4));
    flmakerShape.setFillColor(sf::Color(150, 75, 0));
    flmakerShape.setPosition({29.0f * cellSize,16.0f * cellSize});
    window.draw(flmakerShape);

    sf::RectangleShape pmakerShape(sf::Vector2f(cellSize*4,cellSize*2));
    pmakerShape.setFillColor(sf::Color(75, 0, 130));
    pmakerShape.setPosition({22.0f * cellSize,22.0f * cellSize});
    window.draw(pmakerShape);

    sf::RectangleShape shipShape(sf::Vector2f(cellSize - 4, cellSize -8));
    shipShape.setFillColor(sf::Color::Magenta);

    for (const auto& ship: ships) {
        if (ship.isActive()) {
            shipShape.setPosition({ship.getX(), ship.getY()});
            window.draw(shipShape);
        }
    }

    sf::RectangleShape pShape(sf::Vector2f( cellSize - 4, cellSize - 4));
    pShape.setFillColor(sf::Color::White);
    pShape.setPosition({player.getX() * cellSize + 2, player.getY() * cellSize + 2});
    window.draw(pShape);
}
