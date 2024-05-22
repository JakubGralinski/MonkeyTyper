#pragma once
#include <SFML/Graphics.hpp>
#define MAX_NUMBER_OF_ITEMS 4


class Menu {
public:
    Menu(float width, float height);
    ~Menu();

    void draw(sf::RenderWindow &window);
    void MoveUp();
    void MoveDown();
    int getPressedItem() { return selectedItemIndex; }
    bool isMouseOver(sf::Vector2i mousePos);

    sf::Text menu[MAX_NUMBER_OF_ITEMS];
private:
    int selectedItemIndex;
    sf::Font font;

    // Title text and animation properties
    sf::Font titleFont;
    sf::Text titleText;
    float animationTime;
    const float animationSpeed = 2.0f;
    const float scaleFactor = 0.05f;


    // Hover color
    sf::Color hoverColor; // Orange

    sf::Clock clock; // Measuring time for animation
};
