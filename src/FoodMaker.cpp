#include "../include/Foodmaker.h"
#include <iostream>
#include <wallet.h>

Foodmaker::Foodmaker(int x, int y) {
    name_ = "Food & Drinks Station";
    position_ = Position(x, y);
}

std::string Foodmaker::getType() const {
    return "FoodAndDrinksStation";
}

void Foodmaker::setMode(FoodDrinksMode mode) {
    currentMode_ = mode;
    std::cout << name_ << " switched to: " << getModeName() << "\n";
}

FoodDrinksMode Foodmaker::getMode() const {
    return currentMode_;
}

std::string Foodmaker::getModeName() const {
    switch(currentMode_) {
        case FoodDrinksMode::Food: return "Food";
        case FoodDrinksMode::Drinks: return "Drinks";
        default: return "Unknown";
    }
}

Resource::Type Foodmaker::getResourceType() const {
    switch(currentMode_) {
        case FoodDrinksMode::Food: return Resource::Type::Food;
        case FoodDrinksMode::Drinks: return Resource::Type::Drinks;
        default: return Resource::Type::Food;
    }
}

int Foodmaker::getCurrentPrice() const {
    switch(currentMode_) {
        case FoodDrinksMode::Food:
            return 30 + (level_ - 1) * 12;
        case FoodDrinksMode::Drinks:
            return 20 + (level_ - 1) * 8;
        default:
            return 0;
    }
}

bool Foodmaker::produce(InformationPlayer& player) {
    int price = getCurrentPrice();
    Resource::Type resourceType = getResourceType();
    std::string typeName = getModeName();

    wallet& wallet = player.getWal();
    if (wallet.getBal() < price) {
        std::cout << "Not enough money! Need " << price
                  << ", have " << wallet.getBal() << "\n";
        return false;
    }

    wallet.withdraw(price);

    player.addResource(resourceType, 1);

    std::cout << "🍳 " << name_ << " (Lvl " << level_ << ") produced 1 " << typeName << "\n";
    std::cout << "   Price: " << price << " credits\n";
    std::cout << "   Money left: " << wallet.getBal() << "\n";
    std::cout << "   Total " << typeName << ": "
              << player.getAmountResource(resourceType) << "\n";

    return true;
}

int Foodmaker::getUpgradeCost() const {
    return 180 * level_;
}

void Foodmaker::showAvailableModes() {
    std::cout << "\n=== Available Modes ===\n";
    std::cout << "0. Food\n";
    std::cout << "1. Drinks\n";
    std::cout << "=======================\n";
}