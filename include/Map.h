#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "InformationPlayer.h"
#include "Ship.h"
#include "Appliance.h"
class Map {
    sf::Font font;
public:
    bool loadResources();
    static void draw(sf::RenderWindow& window, const std::vector<std::string>& map, const InformationPlayer& player, const std::vector<Ship>& ships);
    void drawWindow(sf::RenderWindow &window);
};
