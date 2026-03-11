#include "GameEngine.h"
#include <iostream>
#include "include/GameEngine.h"
#include <string>
#include "include/InformationPlayer.h"
InformationPlayer createPlayer() {
    std::string name;
    std::cout << "=== CHARACTER CREATION ===\n\n";
    std::cout << "Enter player name: ";
    std::getline(std::cin, name);
    std::cout << "\n Character created!\n";
    std::cout << "Starting balance: 100 credits\n";

    return InformationPlayer(name, 5.0f, 5.0f);
}
void showPlayerStatus(const InformationPlayer& player) {
    std::cout << "\n======= PLAYER STATUS =======\n";
    player.print();
    std::cout << "==================================\n";
}

int main() {
    InformationPlayer player("alex",5.0f, 5.0f);
    GameEngine engine;

    engine.run(player);

    return 0;
}