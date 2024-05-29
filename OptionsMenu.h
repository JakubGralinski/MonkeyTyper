#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class OptionsMenu {
public:
    OptionsMenu(float width, float height) {
        if (!font.loadFromFile("/Users/jakubgralinski/CLionProjects/MonkeyTyper/fonts/FiraCode-Bold.ttf")) {
            // Handle error
        }

        // Configure Fonts button
        buttons[0].setFont(font);
        buttons[0].setCharacterSize(24);
        buttons[0].setString("Fonts");
        buttons[0].setPosition(sf::Vector2f(width / 2.0f - buttons[0].getGlobalBounds().width / 2.0f, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

        // Configure Speed button
        buttons[1].setFont(font);
        buttons[1].setCharacterSize(24);
        buttons[1].setString("Speed");
        buttons[1].setPosition(sf::Vector2f(width / 2.0f - buttons[1].getGlobalBounds().width / 2.0f, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

        // Configure Back button
        buttons[2].setFont(font);
        buttons[2].setCharacterSize(24);
        buttons[2].setString("Back");
        buttons[2].setPosition(sf::Vector2f(width / 2.0f - buttons[2].getGlobalBounds().width / 2.0f, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));

        selectedItemIndex = 0;
    }

    void draw(sf::RenderWindow& window) {
        for (int i = 0; i < MAX_NUMBER_OF_ITEMS; ++i) {
            window.draw(buttons[i]);
        }
    }

    void MoveUp() {
        if (selectedItemIndex - 1 >= 0) {
            selectedItemIndex--;
        }
    }

    void MoveDown() {
        if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS) {
            selectedItemIndex++;
        }
    }

    int GetPressedItem() {
        return selectedItemIndex;
    }

private:
    int selectedItemIndex;
    sf::Font font;
    sf::Text buttons[3];
    static const int MAX_NUMBER_OF_ITEMS = 3;
};
