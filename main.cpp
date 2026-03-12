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