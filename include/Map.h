#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "InformationPlayer.h"
#include "Ship.h"
#include "Appliance.h"
#include "Foodmaker.h"

enum class WindowType { Food, Info, Warning };

enum class FoodMakerWindow { Food, Drinks, Level, CurrentPrice, Name };

class Map {
    sf::Font font;
    float cellSize = 28.0f;
    sf::RectangleShape text_window;
    sf::Text text;
    FoodMakerWindow m_currentFoodMode = FoodMakerWindow::Name;
    Foodmaker foodmaker;
public:
    Map();
    //bool loadResources();
    static void draw(sf::RenderWindow& window, const std::vector<std::string>& map, const InformationPlayer& player, const std::vector<Ship>& ships);
    void drawWindow(sf::RenderWindow &window);
    void prepareWindow(WindowType type);
};
