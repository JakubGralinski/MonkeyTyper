#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>

class OptionsMenu {
public:
    OptionsMenu(float width, float height);
    void draw(sf::RenderWindow &window);
    bool isMouseOverButton(sf::RenderWindow &window, sf::Text &button);
    int GetPressedItem();
    void HandleClick();

    sf::Text buttons[4];
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

    /*int& gameState;  // Reference to the game state
    const int& showingMenuState;  // Reference to the showing menu state*/

    std::string extractFontName(const std::string& fontPath);
};

#endif // OPTIONS_MENU_H
