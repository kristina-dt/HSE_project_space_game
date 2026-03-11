#include "../include/GameEngine.h"

GameEngine::GameEngine(){
    keepRunning=true;
    map = {
        "------------------------------",
        "|                            |",
        "|                            |",
        "|                            |",
        "|                            |",
        "|                            |",
        "|                            |",
        "|                            |",
        "|                            |",
        "|                            |",
        "|                            |",
        "|                            |",
        "|                            |",
        "|                            |",
        "|                            |",
        "|                            |",
        "------------------------------"
    };
}
 void GameEngine::movements(InformationPlayer& player) {
    float speed = 0.3f;
    float nx = player.getX();
    float ny = player.getY();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) ny -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) ny += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) nx -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) nx += speed;

    if (ny >= 1.0f && ny < (float)map.size() - 1.0f &&
        nx >= 1.0f && nx < (float)map[0].size() - 1.0f) {
        if (map[(int)ny][(int)nx] == ' ') {
            player.SetPosition(nx, ny);
        }
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




