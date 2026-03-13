#include "../include/Map.h"
#include "GameEngine.h"
#include <iostream>
Map::Map(): text(font, "", 18), foodmaker(1,18){
    font.openFromFile("fonts/DeluxePaint-Regular.otf");

    text.setFillColor(sf::Color::White);
    text.setPosition({110.f, 110.f});

    text_window.setSize({300.f, 200.f});
    text_window.setFillColor(sf::Color(0, 0, 0, 200));
    text_window.setOutlineThickness(2.f);
    text_window.setOutlineColor(sf::Color::White);
    text_window.setPosition({100.f, 100.f});
}

// bool Map::loadResources() {
//     return font.openFromFile("fonts/DeluxePaint-Regular.otf");
// }
void Map::prepareWindow(WindowType type) {
    switch (type) {
        case WindowType::Food:
                switch (m_currentFoodMode) {
                    case FoodMakerWindow::Name:
                        text.setString("=== BUILDING ===\n\nName: Food Maker");
                        break;

                    case FoodMakerWindow::Food:
                        text.setString("=== FOOD MODE ===\n\nCurrent mode: "
                            + foodmaker.getModeName() + "\nFood price: " + std::to_string(foodmaker.getCurrentPrice()) + " credits");
                        break;

                    case FoodMakerWindow::Drinks:
                        text.setString("=== DRINKS MODE ===\n\nCurrent mode:"
                            + foodmaker.getModeName() +"\nDrinks price: " + std::to_string(foodmaker.getCurrentPrice()) +  " credits" );
                        break;

                    case FoodMakerWindow::Level:
                        text.setString("=== LEVEL INFO ===\n\nLevel:"
                            + std::to_string(foodmaker.getLevel())
                            + "\nUpgrade cost: " +  std::to_string(foodmaker.getUpgradeCost()) +  " credits");
                        break;

                    case FoodMakerWindow::CurrentPrice:
                        text.setString("=== PRICE INFO ===\n\nCurrent mode:"
                            + foodmaker.getModeName()+ "\nPrice: " + std::to_string(foodmaker.getCurrentPrice()) + " credits" );
                        break;
                }
            }
    }


void Map::drawWindow(sf::RenderWindow &window) {
    window.draw(text_window);
    window.draw(text);
}

void Map::draw(sf::RenderWindow &window, const std::vector<std::string> &map, const InformationPlayer &player, const std::vector<Ship>& ships) {
    const float cellSize=28.0f;
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
    pShape.setPosition({player.getX() * cellSize, player.getY() * cellSize});
    window.draw(pShape);
}
