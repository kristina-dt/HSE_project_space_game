#include "../include/PartAssembler.h"
#include <iostream>

PartAssembler::PartAssembler(int x, int y) {
    name_ = "PartAssembler";
    position_ = Position(x, y);
}

std::string PartAssembler::getType() const {
    return "PartAssembler";
}

void PartAssembler::setMode(PartAssemblerMode mode) {
    currentMode_ = mode;
    std::cout << name_ << " switched to: " << getModeName() << "\n";
}

PartAssemblerMode PartAssembler::getMode() const {
    return currentMode_;
}

std::string PartAssembler::getModeName() const {
    switch(currentMode_) {
        case PartAssemblerMode::Details: return "Details";
        case PartAssemblerMode::Decorations: return "Decorations";
        default: return "Unknown";
    }
}

int PartAssembler::getCurrentPrice() const {
    switch(currentMode_) {
        case PartAssemblerMode::Details:
            return 20 + (level_ - 1) * 15;
        case PartAssemblerMode::Decorations:
            return 45 + (level_ - 1) * 25;
        default:
            return 0;
    }
}

bool PartAssembler::produce(InformationPlayer& player) {
    int price = getCurrentPrice();
    Resource::Type resourceType = getResourceType();
    std::string productName = getModeName();

    player.addResource(resourceType, 1);

    std::cout << name_ << " (Lvl " << level_ << ") produced 1 " << productName << "\n";
    std::cout << "   Price: " << price << " credits\n";
    std::cout << "   Total " << productName << ": "
              << player.getAmountResource(resourceType) << "\n";

    return true;
}

int PartAssembler::getUpgradeCost() const {
    return 50 * level_;
}

void PartAssembler::showAvailableModes() {
    std::cout << "\n=== Available Modes ===\n";
    std::cout << "0. Details (basic components)\n";
    std::cout << "1. Decorations (luxury items)\n";
    std::cout << "=======================\n";
}

Resource::Type PartAssembler::getResourceType() const {
    switch(currentMode_) {
        case PartAssemblerMode::Details:
            return Resource::Type::Details;
        case PartAssemblerMode::Decorations:
            return Resource::Type::Decorations;
        default:
            return Resource::Type::Details;
    }
}
int PartAssembler::getLevel() const {
    return level_;
}