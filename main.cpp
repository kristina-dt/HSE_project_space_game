#include "GameEngine.h"
#include <iostream>
#include "include/GameEngine.h"
#include <string>
#include <random>

#include "ResourceOrder.h"
#include "include/InformationPlayer.h"
#include "include/ProductionManager.h"
#include "include/NotEnoughMoney.h"
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
        auto station = manager.getAppliance(i);
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
std::unique_ptr<Order> generateRandomOrder() {
    static std::mt19937 rng(std::random_device{}());

    std::uniform_int_distribution<int> resourceDist(0, 4);
    std::uniform_int_distribution<int> amountDist(1, 5);

    Resource::Type type = static_cast<Resource::Type>(resourceDist(rng));
    int amount = amountDist(rng);
    int reward = amount * 20 + 10;

    return std::make_unique<ResourceOrder>(type, amount, reward);
}
std::string getOrderTitle(int orderNumber) {
    switch (orderNumber) {
        case 1:
            return "FIRST ORDER!";
        case 2:
            return "SECOND ORDER!";
        case 3:
            return "THIRD ORDER!";
        default:
            return "NEW ORDER!";
    }
}
std::string getResourceName(Resource::Type type) {
    switch (type) {
        case Resource::Type::Fuel:
            return "Fuel";
        case Resource::Type::Food:
            return "Food";
        case Resource::Type::Drinks:
            return "Drinks";
        case Resource::Type::Details:
            return "Details";
        case Resource::Type::Decorations:
            return "Decorations";
    }
    return "Unknown";
}

void processOrder(InformationPlayer& player, ProductionManager& manager, Ship& ship, int orderNumber) {
    if (!ship.hasOrder()) {
        std::cout << "\nNo active order.\n";
        return;
    }

    Order* order = ship.getOrder();
    if (!order) {
        std::cout << "\nOrder is missing.\n";
        return;
    }

    std::cout << "\n=== " << getOrderTitle(orderNumber) << " ===\n";
    std::cout << "Order #" << orderNumber << "\n";
    std::cout << order->describe() << "\n";

    Resource::Type neededType = order->resourceType();
    int neededAmount = order->amount();
    int playerAmount = player.getAmountResource(neededType);

    std::cout << "You have: " << playerAmount
              << " units of " << getResourceName(neededType) << "\n";

    if (playerAmount < neededAmount) {
        std::cout << "\nNot enough resources!\n";
        std::cout << "You need " << (neededAmount - playerAmount) << " more units.\n";
        return;
    }

    std::string answer;
    std::cout << "\nYou have enough resources!\n";
    std::cout << "Complete this order? (yes/no): ";
    std::cin >> answer;

    if (answer == "yes" || answer == "y") {
        bool sold = player.sell(neededType, neededAmount);

        if (!sold) {
            std::cout << "\nCould not complete the order.\n";
            return;
        }

        int reward = ship.completeOrderAndGetReward();
        player.getWal().deposit(reward);

        std::cout << "\nORDER COMPLETED!\n";
        std::cout << "You earned: " << reward << " credits!\n";

        AskStatus(player, manager);
    } else {
        std::cout << "\nOrder declined.\n";
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
    for (int i = 1; i <= 3; ++i) {
        Ship ship(
            Ship::START_X,
            5.0f,
            Ship::DEFAULT_SPEED,
            generateRandomOrder()
        );

        processOrder(player, manager, ship, i);
    }

    std::cout << "\n=== FINAL STATUS ===\n";
    showPlayerStatus(player, manager);

    std::cout << "\nGame finished.\n";
    // InformationPlayer player("alex",15.5f, 20.0f);
    // GameEngine engine;
    // engine.run(player);

    return 0;
}