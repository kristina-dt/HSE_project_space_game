#include "GameEngine.h"
#include <iostream>
#include "include/GameEngine.h"
#include <string>
#include <random>
#include <algorithm>
#include "ResourceOrder.h"
#include "include/InformationPlayer.h"
#include "include/ProductionManager.h"
#include "include/NotEnoughMoney.h"

std::string getYesNoInput(const std::string& prompt) {
    std::string answer;
    while (true) {
        std::cout << prompt;
        std::cin >> answer;

        std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

        if (answer == "yes" || answer == "no") {
            return answer;
        }

        std::cout << "Invalid input! Please enter 'yes' or 'no'.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}
InformationPlayer createPlayer() {
    std::string name;
    std::cout << "=== CHARACTER CREATION ===\n";
    std::cout << "Enter player name: ";
    std::getline(std::cin, name);
    std::cout << "\n Character created!\n";
    std::cout << "Starting balance: 100 credits\n";

    return InformationPlayer(name, 15.5f, 20.0f);
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
void AskStatus(const InformationPlayer& player, ProductionManager& manager) {
    std::string answer = getYesNoInput("\nDo you want to see your current status? (yes/no): ");

    if (answer == "yes" || answer == "y") {
        std::cout << "\n======= CURRENT STATUS =======\n";
        showPlayerStatus(player, manager);
    } else {
        std::cout << "Status is hidden\n";
    }
}
std::unique_ptr<Order> generateRandomOrder(const InformationPlayer& player) {
    static std::mt19937 rng(std::random_device{}());

    std::uniform_int_distribution<int> resourceDist(0, 4);
    std::uniform_int_distribution<int> amountDist(1, 5);

    Resource::Type type = static_cast<Resource::Type>(resourceDist(rng));
    int amount = amountDist(rng);
    int reward = amount * player.getSellPrice(type);
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
        std::cout << "Order missed.\n";
        return;
    }

    std::string answer = getYesNoInput("\nYou have enough resources!\nComplete this order? (yes/no): ");

    if (answer == "yes" || answer == "y") {
        bool sold = player.sell(neededType, neededAmount);

        if (!sold) {
            std::cout << "\nCould not complete the order.\n";
            return;
        }
        int pricePerUnit = player.getSellPrice(neededType);
        int calculatedReward = neededAmount * pricePerUnit;
        int reward = ship.completeOrderAndGetReward();
        player.getWal().deposit(calculatedReward);

        std::cout << "\nORDER COMPLETED!\n";
        std::cout << "You earned: " << calculatedReward<< " credits!\n";

        AskStatus(player, manager);
    } else {
        std::cout << "\nOrder declined.\n";
    }
}
bool upgradeStation(ProductionManager& productionManager, InformationPlayer& player, size_t stationIndex) {
    if (stationIndex >= productionManager.getCount()) {
        std::cout << "Error: appliance with index " << stationIndex << " does not exist!\n";
        std::cout << "   Available index: 0-" << productionManager.getCount() - 1 << "\n";
        return false;
    }

    std::shared_ptr<Appliance> appliance = productionManager.getAppliance(stationIndex);
    if (!appliance) {
        std::cout << "Error: could not get access to appliance\n";
        return false;
    }

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

    std::string productName = productionManager.getProductName(stationIndex);
    int productPrice = productionManager.getProductPrice(stationIndex);

    if (stationType == "FuelMaker" || stationType == "Fuel Synthesizer") {
        std::cout << "   • Current cost of fuel: " << productPrice << "\n";
    }
    else if (stationType == "PartAssembler") {
        std::cout << "   • Current mode: " << productName << "\n";
        std::cout << "   • Current cost of " << productName << ": " << productPrice << " credits\n";
    }
    else if (stationType == "FoodAndDrinksStation") {
        std::cout << "   • Current mode: " << productName << "\n";
        std::cout << "   • Current cost of " << productName << ": " << productPrice << " credits\n";
    }

    std::cout << "   • Cost of upgrade: " << upgradeCost << " credits\n";
    std::cout << "   • Your balance: " << playerBalance << " credits\n";

    if (playerBalance < upgradeCost) {
        std::cout << "\n You don't have enough money for upgrade!\n";
        std::cout << "   You need: " << (upgradeCost - playerBalance) << " credits\n";
        return false;
    }

    if (currentLevel >= 10) {
        std::cout << "\n Appliance reached maximum level (10)!\n";
        return false;
    }

    std::string confirm = getYesNoInput("\nConfirm upgrade (yes/no): ");

    if (confirm != "yes" && confirm != "y") {
        std::cout << "Upgrade cancelled.\n";
        return false;
    }

    try {
        player.getWal().withdraw(upgradeCost);

        int oldLevel = currentLevel;
        appliance->upgrade();
        int newPrice = productionManager.getProductPrice(stationIndex);

        if (productName == "Fuel")
            player.sellSetPrice(Resource::Type::Fuel, newPrice);
        else if (productName == "Food")
            player.sellSetPrice(Resource::Type::Food, newPrice);
        else if (productName == "Drinks")
            player.sellSetPrice(Resource::Type::Drinks, newPrice);
        else if (productName == "Details")
            player.sellSetPrice(Resource::Type::Details, newPrice);
        else if (productName == "Decorations")
            player.sellSetPrice(Resource::Type::Decorations, newPrice);
        int newLevel = appliance->getLevel();

        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << " UPGRADE SUCCESSFUL!\n";
        std::cout << std::string(50, '=') << "\n";
        std::cout << "   • Appliance: " << stationType << "\n";
        std::cout << "   • Level: " << oldLevel << " → " << newLevel << "\n";

        std::string newProductName = productionManager.getProductName(stationIndex);

        std::cout << "\n New prices:\n";

        if (stationType == "FuelMaker" || stationType == "Fuel Synthesizer") {
            std::cout << "   • New cost of fuel: " << newPrice << " credits\n";
        }
        else if (stationType == "PartAssembler") {
            std::cout << "   • New cost of " << newProductName << ": " << newPrice << " credits\n";
        }
        else if (stationType == "FoodAndDrinksStation") {
            std::cout << "   • New cost of " << newProductName << ": " << newPrice << " credits\n";
        }

        return true;

    } catch (const NotEnoughMoney& e) {
        std::cout << "\n Payment error: " << e.what() << "\n";
        return false;
    } catch (const std::exception& e) {
        std::cout << "\n Unexpected error: " << e.what() << "\n";
        return false;
    }
}


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
void askForUpgrade(ProductionManager& manager, InformationPlayer& player) {
    std::cout << "\n=== UPGRADE AVAILABLE ===\n";

    bool avai = false;
    for (size_t i = 0; i < manager.getCount(); ++i) {
        auto station = manager.getAppliance(i);
        if (!station) continue;
        int cost = station->getUpgradeCost();
        int level = station->getLevel();
        int balance = player.getWal().getBal();
        if (balance >= cost && level < 10) {
            avai = true;
            std::cout << "Station " << i
                      << " (" << station->getType() << ") "
                      << "can be upgraded for "
                      << cost << " credits.\n";
        }
    }
    if (!avai) {
        std::cout << "No upgrades available right now.\n";
        return;
    }
    std::string answer = getYesNoInput("\nDo you want to upgrade something? (yes/no): ");

    if (answer == "yes" || answer == "y") {
        size_t index;
        std::cout << "Enter station index to upgrade: ";
        std::cin >> index;
        upgradeStation(manager, player, index);
    }
}
bool ReplShop(InformationPlayer& player) {
    std::cout << "\n=== REPLENISHMENT OF SHOP ===\n";
    std::string answer = getYesNoInput("Do you want to restock your shop? (yes/no): ");

    if (answer != "yes" && answer != "y") {
        std::cout << "Replenishment cancelled.\n";
        return false;
    }

    std::cout << "\nAvailable resources to replenishment:\n";
    std::cout << "1. Fuel (current: " << player.getAmountResource(Resource::Type::Fuel) << " units)\n";
    std::cout << "2. Food (current: " << player.getAmountResource(Resource::Type::Food) << " units)\n";
    std::cout << "3. Drinks (current: " << player.getAmountResource(Resource::Type::Drinks) << " units)\n";
    std::cout << "4. Details (current: " << player.getAmountResource(Resource::Type::Details) << " units)\n";
    std::cout << "5. Decorations (current: " << player.getAmountResource(Resource::Type::Decorations) << " units)\n";
    std::cout << "0. Cancel\n";

    int choice;
    std::cout << "\nSelect resource to restock (0-5): ";
    std::cin >> choice;

    if (choice == 0) {
        std::cout << "Replenishment cancelled.\n";
        return false;
    }

    Resource::Type selecType;
    switch (choice) {
        case 1: selecType = Resource::Type::Fuel; break;
        case 2: selecType = Resource::Type::Food; break;
        case 3: selecType = Resource::Type::Drinks; break;
        case 4: selecType = Resource::Type::Details; break;
        case 5: selecType = Resource::Type::Decorations; break;
        default:
            std::cout << "Invalid choice!\n";
            return false;
    }

    std::string typeName = getResourceName(selecType);
    int currentAmount = player.getAmountResource(selecType);
    int basePrice = player.getSellPrice(selecType);
    int restPrice = static_cast<int>(basePrice * 0.7f);

    std::cout << "\n=== REPLENISHMENT " << typeName << " ===\n";
    std::cout << "Current stock: " << currentAmount << " units\n";
    std::cout << "Base selling price: " << basePrice << " credits\n";
    std::cout << "Buying price: " << restPrice << " credits\n";

    int playerBalance = player.getWal().getBal();
    int maxItem = playerBalance / restPrice;

    if (maxItem <= 0) {
        std::cout << "\nYou don't have enough credits to restock!\n";
        std::cout << "Your balance: " << playerBalance << " credits\n";
        std::cout << "Minimum needed: " << restPrice << " credits\n";
        return false;
    }

    std::cout << "Your balance: " << playerBalance << " credits\n";
    std::cout << "You can buy up to " << maxItem << " units\n";
    std::cout << "\nHow many units would you like to buy? (0-" << maxItem << "): ";

    int amountItem;
    std::cin >> amountItem;

    if (std::cin.fail() || amountItem < 0 || amountItem > maxItem) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input! Please enter a number between 0 and " << maxItem << "\n";
        return false;
    }

    if (amountItem == 0) {
        std::cout << "Replenishment cancelled.\n";
        return false;
    }

    int totalCost = amountItem * restPrice;

    std::cout << "\nRestock summary:\n";
    std::cout << "  Resource: " << typeName << "\n";
    std::cout << "  Amount: " << amountItem << " units\n";
    std::cout << "  Price per unit: " << restPrice << " credits\n";
    std::cout << "  Total cost: " << totalCost << " credits\n";
    std::cout << "  Your balance after: " << (playerBalance - totalCost) << " credits\n";
    std::string confirm = getYesNoInput("\nConfirm restock? (yes/no): ");

    if (confirm != "yes" && confirm != "y") {
        std::cout << "Replenishment cancelled.\n";
        return false;
    }


    try {
        player.getWal().withdraw(totalCost);

        player.addResource(selecType, amountItem);

        std::cout << "\n COMPLETED SUCCESSFULLY!\n";
        std::cout << "  Added " << amountItem << " units of " << typeName << "\n";
        std::cout << "  New stock: " << player.getAmountResource(selecType) << " units\n";
        std::cout << "  New balance: " << player.getWal().getBal() << " credits\n";
        return true;

    } catch (const NotEnoughMoney& e) {
        std::cout << "\nPayment error: " << e.what() << "\n";
        return false;
    }
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
            generateRandomOrder(player)
        );

        processOrder(player, manager, ship, i);
        ReplShop(player);
        askForUpgrade(manager, player);
    }

    std::cout << "\n=== FINAL STATUS ===\n";
    showPlayerStatus(player, manager);

    std::cout << "\nGame finished.\n";
    //InformationPlayer player("alex",15.5f, 20.0f);
    GameEngine engine;
    Map map;
    engine.run(player,map);

    return 0;
}