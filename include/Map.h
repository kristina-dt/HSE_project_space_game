#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "InformationPlayer.h"
class Map {
public:
    static void WholeMap(const std::vector<std::string>& map, const InformationPlayer& player);
};
