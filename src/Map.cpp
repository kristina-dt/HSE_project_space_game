#include "../include/Map.h"
#include "GameEngine.h"
#include <iostream>

void Map::WholeMap(const std::vector<std::string> &map, const InformationPlayer& player) {
    for (int i = 0; i < GameEngine::HEIGHT; ++i) {
        for (int j = 0; j < GameEngine::WIDTH; ++j) {
            if (i==(player.getY()) && j==(player.getX())) {
                std::cout<<"[^]";
                j+=2;
            }
            if (j < GameEngine::WIDTH) {
                std::cout<<map[i][j];
            }
        }
        std::cout<<'\n';
    }
}