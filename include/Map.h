#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "InformationPlayer.h"
#include "Ship.h"
#include "Foodmaker.h"
#include "PartAssembler.h"
#include "FuelMaker.h"

enum class WindowType { Food, Fuel, PartAssembler };
enum class FoodMakerWindow { Food, Drinks, Level, CurrentPrice, Name };
enum class PartAssemblerWindow { Details, Decorations, Level, CurrentPrice, Name };

class Map {
    sf::Font font;
    float cellSize = 28.0f;
    sf::RectangleShape text_window;
    sf::Text text;
    FoodMakerWindow m_currentFoodMode = FoodMakerWindow::Name;
    Foodmaker foodmaker;
    PartAssembler partassembler;
    FuelMaker fuelmaker;
    int foodNumber = 7;
    int fuelNumber = 5;
    int partNumber = 5;
public:
    Map();
    int getFn() {
        return foodNumber;
    }
    int getFln() {
        return fuelNumber;
    }
    int getPn() {
        return partNumber;
    }
    void updateAmount(WindowType type);

    static void draw(sf::RenderWindow& window, const std::vector<std::string>& map, const InformationPlayer& player, const std::vector<Ship>& ships);
    void drawWindow(sf::RenderWindow &window);
    void prepareWindow(WindowType type);
};
