#include "GameEngine.h"
#include <iostream>
#include "include/GameEngine.h"
#include <string>
#include <random>
#include "include/InformationPlayer.h"
#include "include/ProductionManager.h"
InformationPlayer createPlayer() {
    std::string name;
    std::cout << "=== CHARACTER CREATION ===\n";
    std::cout << "Enter player name: ";
    std::getline(std::cin, name);
    std::cout << "\n Character created!\n";
    std::cout << "Starting balance: 100 credits\n";

    return InformationPlayer(name, 5.0f, 5.0f);
}
void showPlayerStatus(const InformationPlayer& player, ProductionManager& manager) {
    std::cout << "\n======= PLAYER STATUS =======\n";
    player.print();
    std::cout << "\n=== STATIONS ===\n";
    for (size_t i = 0; i < manager.getCount(); ++i) {
        Appliance* station = manager.getAppliance(i);
        if (station) {
            std::cout << "Station " << i << ": " << station->getType()
                      << " (Level " << station->getLevel() << ")\n";
        }
    }
    std::cout << "==================================\n";
}
void AskStatus(const InformationPlayer& player,  ProductionManager& manager) {
    std::string answer;
    std::cout << "\n Do you want to see your current status? (yes/no): ";
    std::cin >> answer;
    if (answer == "yes" || answer == "y") {
        std::cout << "\n======= CURRENT STATUS =======\n";
        showPlayerStatus(player, manager);
    }
    else if (answer == "no" || answer == "n") {
        std::cout<<"Status is hidden\n";
    }
    else {
        std::cout<<"Invalid input. Please enter yes or no.\n";
    }

}
bool upgradeStation(ProductionManager& productionManager, InformationPlayer& player, size_t stationIndex) {
    if (stationIndex >= productionManager.getCount()) {
        std::cout << "Eroor: appliance with index " << stationIndex << " does not exist!\n";
        std::cout << "   Availiable index: 0-" << productionManager.getCount() - 1 << "\n";
        return false;
    }//checking if index of appliance is correct
    std::shared_ptr<Appliance> appliance = productionManager.getAppliance(stationIndex);
    if (!appliance) {
        std::cout << "Error: could not get access to appliance\n";
        return false;
    } //get pointer on appliance
    std::string stationType = appliance->getType();
    int currentLevel = appliance->getLevel();
    int upgradeCost = appliance->getUpgradeCost();
    int playerBalance = player.getWal().getBal();

    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << " UPGRADE APPLIANCE \n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << "Information about appliance:\n";
    std::cout << "   • Type: " << stationType << "\n";
    std::cout << "   • Current level: " << currentLevel << "\n";
    std::cout << "   • Next level: " << (currentLevel + 1) << "\n";

    //For appliance that has type
    if (stationType == "PartAssembler") {
        std::shared_ptr<PartAssembler> assembler = std::dynamic_pointer_cast<PartAssembler>(appliance);
        if (assembler) {
            std::cout << "   • Current mode: " << assembler->getModeName() << "\n";
            std::cout << "   • Current cost of a product: " << assembler->getCurrentPrice() << "\n";
        }
    }
    else if (stationType == "FoodAndDrinksStation") {
        std::shared_ptr<Foodmaker> foodmaker = std::dynamic_pointer_cast<Foodmaker>(appliance);
        if (foodmaker) {
            std::cout << "   • Current mode: " << foodmaker->getModeName() << "\n";
            std::cout << "   • Current cost of a product: " << foodmaker->getCurrentPrice() << "\n";
        }
    }
    else if (stationType == "FuelMaker" || stationType == "Fuel Synthesizer") {
        std::shared_ptr<FuelMaker> fuelMaker = std::dynamic_pointer_cast<FuelMaker>(appliance);
        if (fuelMaker) {
            std::cout << "   • Current cost of fuel: " << fuelMaker->getFuelPrice() << "\n";
        }
    }
    std::cout << "   • Cost of upgrade: " << upgradeCost << " credits\n";
    std::cout << "   • Your balance: " << playerBalance << " credits\n";

    if (playerBalance < upgradeCost) {
        std::cout << "\n You don't have enough money for upgrade!\n";
        std::cout << "   You need: " << (upgradeCost - playerBalance) << " credits\n";
        return false;
    }
    if (currentLevel >= 10) {
        std::cout << "\n Appliance reached maximum level(10)!\n";
        return false;
    }
    std::cout << "\n Confirm upgrade: Yes/No";
    std::string confirm;
    std::cin >> confirm;

    if (confirm != "yes" && confirm != "Yes" && confirm != "YES") {
        std::cout << " Upgrade denied.\n";
        return false;
    }
    try {
        player.getWal().withdraw(upgradeCost);

        int oldLevel = currentLevel;
        appliance->upgrade();
        int newLevel = appliance->getLevel();

        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << " UPGRADE IS SUCCESSFUL!\n";
        std::cout << std::string(50, '=') << "\n";
        std::cout << "   • Appliance: " << stationType << "\n";
        std::cout << "   • Level: " << oldLevel << " → " << newLevel << "\n";
        std::cout << "\n New cost of products:\n";

        if (stationType == "PartAssembler") {
            std::shared_ptr<PartAssembler> assembler = std::dynamic_pointer_cast<PartAssembler>(appliance);
            if (assembler) {
                std::cout << "   • New cost of details: " << assembler->getCurrentPrice() << " credits\n";
            }
        }
        else if (stationType == "FoodAndDrinksStation") {
            std::shared_ptr<Foodmaker> foodmaker = std::dynamic_pointer_cast<Foodmaker>(appliance);
            if (foodmaker) {
                std::cout << "   • New cost of products: " << foodmaker->getCurrentPrice() << " credits\n";
            }
        }
        else if (stationType == "FuelMaker" || stationType == "Fuel Synthesizer") {
            std::shared_ptr<FuelMaker> fuelMaker = std::dynamic_pointer_cast<FuelMaker>(appliance);
            if (fuelMaker) {
                std::cout << "   • New cost of fuel: " << fuelMaker->getFuelPrice() << " credits\n";
            }
        }

        return true;

    } catch (const NotEnoughMoney& e) {
        std::cout << "\n Payment error: " << e.what() << "\n";
        return false;
    } catch (const std::exception& e) {
        std::cout << "\n Unexpected error: " << e.what() << "\n";
        return false;
    }
};

bool upgradeStationByType(ProductionManager& productionManager, InformationPlayer& player, const std::string& stationType) {
    for (size_t i = 0; i < productionManager.getCount(); ++i) {
        std::shared_ptr<Appliance> appliance = productionManager.getAppliance(i);
        if (appliance && appliance->getType() == stationType) {
            std::cout << " Found appliance with type " << stationType << " with index " << i << "\n";
            return upgradeStation(productionManager, player, i);
        }
    }

    std::cout << " Appliance of type " << stationType << " is not found!\n";
    return false;
}
int main() {
    InformationPlayer player = createPlayer();
    ProductionManager manager;
    std::cout<<"=== INITIAL STATUS ===";
    showPlayerStatus(player, manager);

    //InformationPlayer player("alex",5.0f, 5.0f);
    GameEngine engine;

    engine.run(player);

    return 0;
}