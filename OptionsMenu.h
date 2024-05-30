#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include "Menu.h"

class OptionsMenu {
public:
    OptionsMenu(float width, float height, Menu& menu);
    void draw(sf::RenderWindow &window);
    bool isMouseOverButton(sf::RenderWindow &window, sf::Text &button);
    int GetPressedItem();
    void HandleClick();

    sf::Text buttons[4];
    std::string speedOptions[3] = {"Slow", "Normal", "Fast"};
    int currentSpeedIndex = 1;

    std::vector<std::string> availableDictionaries;
    int currentDictionaryIndex = 0;

private:
    std::vector<std::string> availableFonts;
    int currentFontIndex;
    sf::Font font;
    struct HoverState {
        bool isHovered;
        sf::Color normalColor;
        sf::Color hoverColor;
    } hoverStates[4];
    int selectedItemIndex;

    Menu& menu; // Reference to the main menu to update its font

    std::string extractFontName(const std::string& fontPath);

    // Default to "Normal"
};

#endif // OPTIONS_MENU_H
