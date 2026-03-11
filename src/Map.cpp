#include "../include/Map.h"
#include "GameEngine.h"
#include <iostream>

void Map::draw(sf::RenderWindow &window, const std::vector<std::string> &map, const InformationPlayer &player) {
    const float cellSize = 28.0f;
    sf::RectangleShape tile(sf::Vector2f(cellSize, cellSize));

    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            tile.setPosition({x * cellSize, y * cellSize});

            if (map[y][x] == '-' || map[y][x] == '|')
                tile.setFillColor(sf::Color(100, 100, 100));
            else
                tile.setFillColor(sf::Color::Black);

            window.draw(tile);
        }
    }

    sf::RectangleShape pShape(sf::Vector2f( cellSize - 4, cellSize - 4));
    pShape.setFillColor(sf::Color::Green);
    pShape.setPosition({player.getX() * cellSize + 2, player.getY() * cellSize + 2});
    window.draw(pShape);
}
