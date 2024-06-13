#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Menu.h"

class OptionsMenu {
public:
    OptionsMenu(float width, float height, Menu& menu);
    void draw(sf::RenderWindow &window);
    void HandleClick();
    int GetPressedItem();
    void updateFont(const sf::Font& newFont);

    bool isMouseOverButton(sf::RenderWindow &window, sf::Text &button);

    sf::Text buttons[5];
    std::vector<std::string> availableDictionaries;
    std::string speedOptions[3] = {"Slow", "Normal", "Fast"};
    int currentSpeedIndex;
    int currentDictionaryIndex;
private:
    std::string extractFontName(const std::string& fontPath);

    sf::Font font;
    Menu& menu;
    int selectedItemIndex;
    int currentFontIndex;
    int currentSizeIndex;
    std::vector<std::string> availableFonts;
    struct HoverState {
        bool isHovered;
        sf::Color normalColor;
        sf::Color hoverColor;
    } hoverStates[5];
    std::string sizeOptions[4] = {"Small", "Normal", "Big", "Random"};
};
