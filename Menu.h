#ifndef MENU_H
#define MENU_H

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
    void updateFont(const sf::Font& font);

private:
    sf::Text menu[4];
    sf::Text scoreTitleText;
    sf::Text scoreText[3];
    sf::Font font;
    sf::Color hoverColor;
    int selectedItemIndex;
    float animationTime;
    float animationSpeed = 2.0f;  // Speed of the title animation
    float scaleFactor = 0.1f;     // Amount to scale the title text by during animation
    sf::Clock clock;              // Clock for timing the animation

    int selectedOptionIndex;      // Track the selected menu option index
};

#endif // MENU_H
