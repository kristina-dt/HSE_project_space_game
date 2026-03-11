#include "../include/GameEngine.h"

GameEngine::GameEngine(){
    keepRunning=true;
    map = {
        "------------------------",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|--|  |---|  |---|  |--|",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "|                      |",
        "------------------------"
    };
}

 void GameEngine::movements(InformationPlayer& player) {
    float speed = 0.1f;
    float oldX = player.getX();
    float oldY = player.getY();
    float nextX = oldX;
    float nextY = oldY;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) nextY -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) nextY += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) nextX -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) nextX += speed;

    if (map[(int)oldY][(int)nextX] == ' ') {
        player.SetPosition(nextX, oldY);
    }
    float currentX = player.getX();
    if (map[(int)nextY][(int)currentX] == ' ') {
        player.SetPosition(currentX, nextY);
    }

}

void GameEngine::run(InformationPlayer& player) {
    sf::RenderWindow window(sf::VideoMode({WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE}), "SpaceGame");
    window.setFramerateLimit(60);

    std::cout << "Window created! Starting loop..." << std::endl;

    while (window.isOpen() && keepRunning) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                std::cout << "Closing window..." << std::endl;
                window.close();
            }
        }
        movements(player);
        window.clear(sf::Color::Black);
        Map::draw(window, map, player);
        window.display();
    }
    std::cout << "Loop finished! Window was open: " << window.isOpen() << std::endl;
}




