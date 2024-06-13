#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
public:
    Menu(float width);
    ~Menu();
    void draw(sf::RenderWindow &window);
    bool isMouseOver(sf::Vector2i mousePos);
    void updateScores(const std::vector<std::string>& scores);
    int getPressedItem();
    void updateFont(const sf::Font& newFont);
    void goBack();

private:
    sf::Font font;
    sf::Text menu[4];
    sf::Text scoreTitleText;
    sf::Text scoreText[3];
    int selectedItemIndex;
    int selectedOptionIndex;
    float animationTime;
    float animationSpeed = 1.0f;
    float scaleFactor = 0.1f;
    sf::Color hoverColor;
    sf::Clock clock;
};
