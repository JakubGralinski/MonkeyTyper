#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Menu {
public:
    Menu(float width, float height);
    ~Menu();

    void draw(sf::RenderWindow &window);
    bool isMouseOver(sf::Vector2i mousePos);
    void updateScores(const std::vector<std::string>& scores);
    int getPressedItem();

private:
    sf::Font font;
    sf::Text menu[4];
    sf::Text optionsItems[3]; // Options menu items
    int selectedItemIndex;
    int selectedOptionIndex;
    sf::Color hoverColor;
    sf::Clock clock;
    float animationTime;
    const float animationSpeed = 2.0f;
    const float scaleFactor = 0.05f;
    sf::Text scoreTitleText;
    sf::Text scoreText[3];
};
