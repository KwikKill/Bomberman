#include "HUD.h"
#include "Game.h"

void HUD::draw(sf::RenderWindow &window, Game &game) {
    // Draw a white rectangle at the bottom of the screen
    sf::RectangleShape rectangle(sf::Vector2f(window.getSize().x, HUD_HEIGHT));
    rectangle.setFillColor(sf::Color(0, 0, 0));
    rectangle.setPosition(0, window.getSize().y - HUD_HEIGHT);
    window.draw(rectangle);

    // Load the font
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        // handle error
    }

    // Draw the number of bombs the player has
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, window.getSize().y - HUD_HEIGHT);
    text.setString("Bombs: " + std::to_string(game.getPlayers()[0].getNumBombs()));
    window.draw(text);

    // Draw the strength of the player
    sf::Text text2;
    text2.setFont(font);
    text2.setCharacterSize(24);
    text2.setFillColor(sf::Color::White);
    text2.setPosition(10, window.getSize().y - HUD_HEIGHT + 25);
    text2.setString("Strenght: " + std::to_string(game.getPlayers()[0].getStrength()));
    window.draw(text2);

    // Draw the number of bombs the AI has
    sf::Text text3;
    text3.setFont(font);
    text3.setCharacterSize(24);
    text3.setFillColor(sf::Color::White);
    text3.setPosition(200, window.getSize().y - HUD_HEIGHT);
    text3.setString("Bombs: " + std::to_string(game.getPlayers()[1].getNumBombs()));
    window.draw(text3);

    // Draw the strength of the AI
    sf::Text text4;
    text4.setFont(font);
    text4.setCharacterSize(24);
    text4.setFillColor(sf::Color::White);
    text4.setPosition(200, window.getSize().y - HUD_HEIGHT + 25);
    text4.setString("Strenght: " + std::to_string(game.getPlayers()[1].getStrength()));
    window.draw(text4);
}