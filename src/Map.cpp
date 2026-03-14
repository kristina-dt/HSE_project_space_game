#include "../include/Map.h"
#include "GameEngine.h"
#include <iostream>
Map::Map(): text(font, "", 12), foodmaker(1,18), fuelmaker(1,3), partassembler(1,3){
    font.openFromFile("fonts/DeluxePaint-Regular.otf");

    text.setFillColor(sf::Color::White);
    text.setPosition({8*28.0f+1.5*28.0f, 84.f+28.0f});

    text_window.setSize({450.f, 400.f});
    text_window.setFillColor(sf::Color(0, 0, 0, 200));
    text_window.setOutlineThickness(2.f);
    text_window.setOutlineColor(sf::Color::White);
    text_window.setPosition({8*28.0f, 3*28.0f});
}

void Map::updateAmount(WindowType type) {
    switch (type) {
        case WindowType::Food:
            foodNumber+=1;
            break;
        case WindowType::Fuel:
            fuelNumber+=1;
            break;
        case WindowType::PartAssembler:
            partNumber+=1;
            break;
    }
}

void Map::prepareWindow(WindowType type) {
    switch (type) {
        case WindowType::Food:
            text.setString("====== FOOD MAKER ======\n\nLevel:"+ std::to_string(foodmaker.getLevel())+
                "\n\nInventory:\n\tFood: "+std::to_string(getFn())+"\n\tDrinks: "+std::to_string(getFn())
                +"\n\nSell Price:\n\tFood: " + std::to_string(foodmaker.getCurrentPrice()) + " credits\n\tDrinks: "
                + std::to_string(foodmaker.getCurrentPrice()) +  " credits\n\nCost of restock:\n\tFood: "+
                std::to_string(foodmaker.getCurrentPrice()*0.7)+"\n\tDrinks: "+std::to_string(foodmaker.getCurrentPrice()*0.7)
                +"\n\nUpgrade cost: " +  std::to_string(foodmaker.getUpgradeCost()) +  " credits");
            break;
        case WindowType::PartAssembler:
            text.setString("==== PART ASSEMBLER ====\n\nLevel:"+ std::to_string(partassembler.getLevel())+
                "\n\nInventory:\n\tDetail: "+std::to_string(getPn())+"\n\tDecoration: "+std::to_string(getPn())
                +"\n\nSell Price:\n\tDetail: " + std::to_string(partassembler.getCurrentPrice()) + " credits\n\tDecoration: "
                + std::to_string(partassembler.getCurrentPrice()) +  " credits\n\nCost of restock:\n\tDetail: "+
                std::to_string(partassembler.getCurrentPrice()*0.7)+"\n\tDecoration: "+std::to_string(partassembler.getCurrentPrice()*0.7)
                +"\n\nUpgrade cost: " +  std::to_string(partassembler.getUpgradeCost()) +  " credits");
            break;
        case WindowType::Fuel:
            text.setString("=== FUEL SYNTHESIZER ===\n\nLevel:"+ std::to_string(fuelmaker.getLevel())+
                "\n\nInventory:\n\tFuel: "+std::to_string(getFln())+"\n\nSell Price:\n\tFuel: "
                + std::to_string(fuelmaker.getCurrentPrice()) + " credits\n\nCost of restock:\n\tFuel: "+
                std::to_string(fuelmaker.getCurrentPrice()*0.7)+
                "\n\nUpgrade cost: " +  std::to_string(fuelmaker.getUpgradeCost()) +  " credits");
            break;
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
