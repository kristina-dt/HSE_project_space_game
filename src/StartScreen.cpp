#include "../include/StartScreen.h"

void StartScreen::deleteChar() {
    std::string t = text.str();
    if (!t.empty()) {
        t.pop_back();
        text.str("");
        text << t;
    }
    input_text.setString(text.str());
}

void StartScreen::setFont(sf::Font &font) {
    input_text.setFont(font);
}

void StartScreen::setPosition(sf::Vector2f pos) {
    textbox.setPosition(pos);
    input_text.setPosition({pos.x + 10.f, pos.y + 17.f});
    def_text.setPosition({pos.x - 3*28.f, pos.y - 6*28.f});
}

std::string StartScreen::getText() {
    return text.str();
}

void StartScreen::drawTo(sf::RenderWindow &window) {
    window.draw(def_text);
    window.draw(textbox);
    window.draw(input_text);
}

void StartScreen::typed(const sf::Event& event) {

    if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        char32_t unicode = textEvent->unicode;

        if (unicode == 8) {
            deleteChar();
        }
        else if (unicode < 128 && unicode >= 32 && text.str().length() < limit) {
            text << static_cast<char>(unicode);
            input_text.setString(text.str() + "_");
        }
    }
}

StartScreen::StartScreen(sf::Font &font): limit(20), input_text(font,"_", 24), def_text(font,"!", 35){
    input_text.setFillColor(sf::Color::White);

    def_text.setFillColor(sf::Color::White);
    def_text.setString("Welcome to the\n  SpaceShip!");

    textbox.setSize(sf::Vector2f(400.f, 60.f));
    textbox.setFillColor(sf::Color::Black);
    textbox.setOutlineColor(sf::Color::White);
    textbox.setOutlineThickness(2.f);
}





