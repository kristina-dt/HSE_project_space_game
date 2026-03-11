#include "GameEngine.h"
#include <iostream>
#include "include/GameEngine.h"
#include "include/InformationPlayer.h"
int main() {
    InformationPlayer player("alex",5.0f, 5.0f);
    GameEngine engine;

    engine.run(player);

    return 0;
}