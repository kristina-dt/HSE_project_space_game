#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "InformationPlayer.h"
#include "Ship.h"
#include "Appliance.h"
class Map {
    std::vector<std::shared_ptr<Appliance>> appliances;
public:
    static void draw(sf::RenderWindow& window, const std::vector<std::string>& map, const InformationPlayer& player, const std::vector<Ship>& ships);
};
