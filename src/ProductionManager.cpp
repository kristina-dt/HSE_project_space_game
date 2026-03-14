#include "../include/ProductionManager.h"
#include <iostream>


ProductionManager::ProductionManager() {
    appliances_.push_back(std::make_shared<FuelMaker>(5, 10));
    appliances_.push_back(std::make_shared<PartAssembler>(8, 12));
    appliances_.push_back(std::make_shared<Foodmaker>(3, 7));
    applianceVariants_.emplace_back(FuelMaker(5, 10));
    applianceVariants_.emplace_back(PartAssembler(8, 12));
    applianceVariants_.emplace_back(Foodmaker(3, 7));

    std::cout << "\nProduction Manager initialized with "
              << appliances_.size() << " appliances\n";
    std::cout << "   Station 0: Fuel Synthesizer at (5, 10)\n";
    std::cout << "   Station 1: Universal Assembler at (8, 12)\n";
    std::cout << "   Station 2: Food & Drinks Station at (3, 7)\n";
}

std::shared_ptr<Appliance> ProductionManager::findApplianceByType(const std::string& type) {
    for (auto& appliance : appliances_) {
        if (appliance->getType() == type) {
            return appliance;
        }
    }
    return nullptr;
}


size_t ProductionManager::getCount() const {
    return appliances_.size();
}

std::shared_ptr<Appliance> ProductionManager::getAppliance(int index) {
    if (index < 0 || index >= static_cast<int>(appliances_.size())) {
        return nullptr;
    }
    return appliances_[index];
}
Position ProductionManager::getAppliancePosition(int index) const {
    if (index >= 0 && index < static_cast<int>(appliances_.size())) {
        return appliances_[index]->getPosition();
    }
    return Position(-1, -1);
}



void ProductionManager::upgradeAppliance(size_t index, InformationPlayer& player) {
    if (index >= appliances_.size()) {
        std::cout << "Invalid station index! Use 0-" << appliances_.size()-1 << "\n";
        return;
    }

    Appliance* app = appliances_[index].get();
    int cost = app->getUpgradeCost();

    std::cout << "\n⚡ UPGRADE ATTEMPT ⚡\n";
    std::cout << "   Station: " << app->getType() << "\n";
    std::cout << "   Current level: " << app->getLevel() << "\n";
    std::cout << "   Upgrade cost: " << cost << " credits\n";

    try {
        player.getWal().withdraw(cost);
        app->upgrade();
        std::cout << " Upgrade successful!\n";
        std::cout << "   New level: " << app->getLevel() << "\n";
        std::cout << "   New product price: " << getProductPrice(index) << " credits\n";

    } catch (const std::exception& e) {
        std::cout << " Upgrade failed: " << e.what() << "\n";
        std::cout << "   You need " << cost << " credits to upgrade\n";
    }
}



std::string ProductionManager::getProductName(int index) const {
    if (index < 0 || index >= static_cast<int>(appliances_.size())) {
        return "Unknown";
    }
    std::string type = appliances_[index]->getType();

    if (type == "Fuel Synthesizer") {
        return "Fuel";
    }
    else if (type == "PartAssembler") {
        const PartAssembler* assembler = static_cast<const PartAssembler*>(appliances_[index].get());
        return assembler->getModeName();
    }
    else if (type == "FoodAndDrinksStation") {
        const Foodmaker* station = static_cast<const Foodmaker*>(appliances_[index].get());
        return station->getModeName();
    }

    return "Unknown";
}

int ProductionManager::getProductPrice(int index) const {
    if (index < 0 || index >= static_cast<int>(appliances_.size())) {
        return 0;
    }

    std::string type = appliances_[index]->getType();

    if (type == "Fuel Synthesizer") {
        const FuelMaker* maker = static_cast<const FuelMaker*>(appliances_[index].get());
        return maker->getCurrentPrice();
    }
    else if (type == "PartAssembler") {
        const PartAssembler* assembler = static_cast<const PartAssembler*>(appliances_[index].get());
        return assembler->getCurrentPrice();
    }
    else if (type == "FoodAndDrinksStation") {
        const Foodmaker* station = static_cast<const Foodmaker*>(appliances_[index].get());
        return station->getCurrentPrice();
    }

    return 0;
}

void ProductionManager::showAllAppliances() const {
    std::cout << "\n=== APPLIANCES ===\n";
    for (size_t i = 0; i < appliances_.size(); ++i) {
        std::cout << i << ": " << appliances_[i]->getType()
                  << " (Level " << appliances_[i]->getLevel() << ")\n";
    }
    std::cout << "==================\n";
}