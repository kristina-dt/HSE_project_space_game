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
    float posX = 2, posY = 3;
    wallet wall;
    std::map<Resource::Type, int> sellPrice;
public:
    void addResource(Resource::Type type, int amount);
    int getResourceAmount(Resource::Type type) const;
    InformationPlayer (const std::string& namepla, float px, float py);
    std::string getName()const;
    constexpr float getX()const{return posX;};
    constexpr float getY()const{return posY;};
    wallet& getWal();
    void SetPosition(float x, float y);
    int getAmountResource(Resource::Type type) const;
    int getSellPrice(Resource::Type type) const;
    void print() const;
};


