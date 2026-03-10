
#include "../include/InformationPlayer.h"
#include "iostream"

InformationPlayer::InformationPlayer(const std::string namepla) : name(namepla), wall(100) {
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
void InformationPlayer::print() const {
    std::cout <<"Player's name: " << name << '\n';
    std::cout << wall << '\n';
    std::cout<<"Inventory:\n";
    for (const auto& res : inventory) {
        std::string typeName;
        switch (res.type) {
            case Resource::Type::Fuel: typeName = "Fuel"; break;
            case Resource::Type::Food: typeName = "Food"; break;
            case Resource::Type::Drinks: typeName = "Drinks"; break;
            case Resource::Type::Details: typeName = "Details"; break;
            case Resource::Type::Decorations: typeName = "Decorations"; break;
        }
        std::cout << "  - " << typeName << ": " << res.amount << " units\n";
    }
    std::cout<<"Fuel sale price: "  << sellPrice.at(Resource::Type::Fuel) << '\n';
    std::cout<<"Food sale price: " << sellPrice.at(Resource::Type::Food)<<'\n';
    std::cout<<"Drinks sale price: " << sellPrice.at(Resource::Type::Drinks)<<'\n';
    std::cout<<"Details sale price: "  << sellPrice.at(Resource::Type::Details)<<'\n';
    std::cout<<"Decorations sale price: " << sellPrice.at(Resource::Type::Decorations)<<'\n';
}