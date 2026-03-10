#pragma once
#include <string>
#include <vector>
#include <map>
#include "wallet.h"
struct Resource {
    enum Type{ Fuel, Food, Drinks, Details, Decorations};
    Type type;
    int amount;
};
class InformationPlayer {
private:
    std::string name;
    std::vector<Resource> inventory;
    int posX, posY;
    wallet wallet;
    std::map<Resource::Type, int> sellPrice;
public:
    InformationPlayer (const std::string namepla, int py, int px, int startMoney);
    std::string getName()const;


};


