
#include "../include/InformationPlayer.h"

InformationPlayer::InformationPlayer(const std::string namepla) : name(namepla), posX(2),posY(3),wall(100) {
    inventory.push_back({Resource::Type::Fuel, 5});
    inventory.push_back({Resource::Type::Food,7});
    inventory.push_back({Resource::Type::Drinks, 7});
    inventory.push_back({Resource::Type::Details, 5});
    inventory.push_back({Resource::Type::Decorations, 0});

    sellPrice[Resource::Type::Decorations] = 45;
    sellPrice[Resource::Type::Details] = 20;
    sellPrice[Resource::Type::Drinks] = 7;
    sellPrice[Resource::Type::Food] = 7;
    sellPrice[Resource::Type::Fuel] = 18;

}

std::string InformationPlayer::getName()const {
    return name;
}
wallet& InformationPlayer::getWal() {return wall;}
void InformationPlayer::SetPosition(int newx, int newy) {
    posX = newx;
    posY = newy;
}
int InformationPlayer::getAmountResource(Resource::Type type) const {
    for (const auto& res : inventory) {
        if (res.type == type) {
            return res.amount;
        }
    }
    return 0;
}
int InformationPlayer::getSellPrice(Resource::Type type) const {
    return sellPrice.at(type);
}