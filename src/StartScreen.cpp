#include "../include/StartScreen.h"

void StartScreen::deleteChar() {
    std::string t = text.str();
    if (!t.empty()) {
        t.pop_back();
        text.str("");
        text << t;
    }
    def_text.setString(text.str());
}

void StartScreen::setFont(sf::Font &font) {
    def_text.setFont(font);
}

void StartScreen::setPosition(sf::Vector2f pos) {
    textbox.setPosition(pos);
    def_text.setPosition({pos.x + 10.f, pos.y + 10.f});
}

std::string StartScreen::getText() {
    return text.str();
}

void StartScreen::drawTo(sf::RenderWindow &window) {
    window.draw(textbox);
    window.draw(def_text);
}

void StartScreen::typed(const sf::Event& event) {

    if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        char32_t unicode = textEvent->unicode;

        if (unicode == 8) {
            deleteChar();
        }
        else if (unicode < 128 && unicode >= 32 && text.str().length() < limit) {
            text << static_cast<char>(unicode);
            def_text.setString(text.str() + "_");
        }
    }
}

StartScreen::StartScreen(sf::Font &font): limit(20), def_text(font,"_", 30){
    def_text.setFillColor(sf::Color::White);

    textbox.setSize(sf::Vector2f(400.f, 60.f));
    textbox.setFillColor(sf::Color::Black);
    textbox.setOutlineColor(sf::Color::White);
    textbox.setOutlineThickness(2.f);
}




