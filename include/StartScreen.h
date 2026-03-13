#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>

class StartScreen {
    sf::Text input_text;
    sf::Text def_text;
    sf::Font font;
    std::ostringstream text;
    sf::RectangleShape textbox;
    int limit;

    void deleteChar();

public:
    StartScreen(sf::Font &font);
    void drawDef();
    void setFont(sf::Font& font);
    void setPosition(sf::Vector2f pos);
    std::string getText();
    void drawTo(sf::RenderWindow& window);
    void typed(const sf::Event& input);
};

