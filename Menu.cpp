#include "Menu.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Menu::Menu(float width) {
    if (!font.loadFromFile("/Users/jakubgralinski/CLionProjects/MonkeyTyper/fonts/FiraCode-Bold.ttf")) {
        std::cerr <<"Error loading font"<< '\n';
    }

    std::ifstream configFile("/Users/jakubgralinski/CLionProjects/MonkeyTyper/config.json");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config file." << '\n';
    }
    json config;
    configFile >> config;
    auto hoverColorJson = config["menuButtonsHoverColor"];
    hoverColor = sf::Color(hoverColorJson[0], hoverColorJson[1], hoverColorJson[2], hoverColorJson[3]);
    float buttonPadding = config["buttonPadding"];
    float buttonInitialYPosition = config["buttonInitialYPosition"];
    configFile.close();

    //Using help of CHAT GPT to create title
    //Create a Title like in Terraria.
    //menu[0].setScale(0.2, 0.2);
    //menu[0].setPosition(window.getSize().x / 2, menu[0].getPosition().y);
    //
    //To create a title in a style similar to Terraria's title screen using C++ and SFML, you need to follow these steps. This example assumes you have a basic SFML setup with a window and a menu texture loaded.
    //1. Load the Font: First, you need to load the font that you'll use for the title.
    //2. Create the Text Object: Then, create an SFML Text object for the title.
    //3. Set the Scale and Position: Adjust the scale and position as specified in your code snippet.
    //4. Draw the Text: Finally, draw the text in the window.
    //Here's a complete example demonstrating how to do this:
    //cpp
    //
    //Copy code
    //#include <SFML/Graphics.hpp>
    //
    //int main() {
    //    // Create the window
    //    sf::RenderWindow window(sf::VideoMode(800, 600), "Terraria-Style Title");
    //
    //    // Load the font
    //    sf::Font font;
    //    if (!font.loadFromFile("path/to/your/font.ttf")) {
    //        // Handle error
    //        return -1;
    //    }
    //
    //    // Create the title text
    //    sf::Text title;
    //    title.setFont(font);
    //    title.setString("Terraria-Style Title");
    //    title.setCharacterSize(100); // Adjust size as needed
    //    title.setFillColor(sf::Color::White);
    //    title.setScale(0.2f, 0.2f); // Set scale to 20%
    //    title.setPosition(window.getSize().x / 2, title.getPosition().y); // Center horizontally
    //
    //    // Center the title horizontally (adjust position to account for scaling)
    //    sf::FloatRect titleBounds = title.getLocalBounds();
    //    title.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    //    title.setPosition(window.getSize().x / 2, 100); // Adjust y-position as needed
    //
    //    // Main loop
    //    while (window.isOpen()) {
    //        sf::Event event;
    //        while (window.pollEvent(event)) {
    //            if (event.type == sf::Event::Closed) {
    //                window.close();
    //            }
    //        }
    //
    //        // Clear the window
    //        window.clear(sf::Color::Black);
    //
    //        // Draw the title
    //        window.draw(title);
    //
    //        // Display the window contents
    //        window.display();
    //    }
    //
    //    return 0;
    //}
    //Explanation:
    //1. Loading the Font: Make sure to replace "path/to/your/font.ttf" with the actual path to your font file.
    //2. Creating the Text Object: sf::Text title creates the text object for your title. The font, string, character size, and color are set.
    //3. Setting Scale and Position:
    //    * title.setScale(0.2f, 0.2f); scales the text down to 20% of its original size.
    //    * title.setPosition(window.getSize().x / 2, 100); sets the position to the center horizontally and 100 pixels down from the top of the window.
    //    * title.setOrigin(titleBounds.width / 2, titleBounds.height / 2); centers the origin of the text for proper horizontal alignment.
    //4. Main Loop: The main loop handles events and draws the title text each frame.
    //This example sets up a simple window with a centered title, scaled to 20% of its original size, similar to the Terraria title style. Adjust the title.setCharacterSize, title.setFillColor, and title.setPosition values to better match the Terraria style as needed.

    menu[0].setFont(font);
    menu[0].setFillColor(sf::Color::Yellow);
    menu[0].setString("Monkey Typer");
    menu[0].setCharacterSize(80);
    menu[0].setStyle(sf::Text::Bold);
    menu[0].setOrigin(menu[0].getLocalBounds().width / 2, menu[0].getLocalBounds().height / 2 + 100);
    menu[0].setPosition(sf::Vector2f(width / 2, buttonInitialYPosition));

    for (int i = 1; i < 4; i++) {
        menu[i].setFont(font);
        menu[i].setFillColor(sf::Color::White); // Initial color
        menu[i].setCharacterSize(40);
        menu[i].setOrigin(menu[i].getLocalBounds().width / 2, menu[i].getLocalBounds().height / 2);
        menu[i].setPosition(sf::Vector2f(width / 2, buttonInitialYPosition + (i * (40 + buttonPadding))));
    }

    menu[1].setString("Play");
    menu[2].setString("Options");
    menu[3].setString("Quit");

    selectedItemIndex = 1; // Start at the first menu item, not the title

    animationTime = 0.0f;

    // Initialize score title text
    scoreTitleText.setFont(font);
    scoreTitleText.setFillColor(sf::Color::White);
    scoreTitleText.setCharacterSize(40);
    scoreTitleText.setString("BEST SCORES");
    scoreTitleText.setOrigin(scoreTitleText.getLocalBounds().width / 2, scoreTitleText.getLocalBounds().height / 2);
    scoreTitleText.setPosition(sf::Vector2f(width / 2, buttonInitialYPosition + (4 * (40 + buttonPadding)) + 60.f)); // Adjust the position as needed

    for (int i = 0; i < 3; i++) {
        scoreText[i].setFont(font);
        scoreText[i].setFillColor(sf::Color::White);
        scoreText[i].setCharacterSize(30);
        scoreText[i].setOrigin(scoreText[i].getLocalBounds().width / 2, scoreText[i].getLocalBounds().height / 2);
        scoreText[i].setPosition(sf::Vector2f(width / 2.2, scoreTitleText.getPosition().y + 40.f + (i * 40))); // Adjust the position as needed
    }
    selectedOptionIndex = 0;
}

Menu::~Menu() {
    // Destructor code, if needed
}

void Menu::draw(sf::RenderWindow &window) {
    float deltaTime = clock.restart().asSeconds();
    animationTime += animationSpeed * deltaTime;
    float scale = 1.0f + scaleFactor * std::sin(animationTime);
    menu[0].setScale(scale, scale);
    menu[0].setPosition(window.getSize().x / 2, menu[0].getPosition().y);

    window.draw(menu[0]);

    for (int i = 1; i < 4; i++) {
        if (isMouseOver(sf::Mouse::getPosition(window)) && selectedItemIndex == i) {
            menu[i].setFillColor(hoverColor);
        } else {
            menu[i].setFillColor(sf::Color::White);
        }
        menu[i].setOrigin(menu[i].getLocalBounds().width / 2, menu[i].getLocalBounds().height / 2);
        menu[i].setPosition(sf::Vector2f(window.getSize().x / 2, menu[i].getPosition().y));

        window.draw(menu[i]);
    }

    // Draw the score title
    window.draw(scoreTitleText);

    // Draw the scores
    for (int i = 0; i < 3; i++) {
        window.draw(scoreText[i]);
    }
}

bool Menu::isMouseOver(sf::Vector2i mousePos) {
    for (int i = 1; i < 4; i++) {
        if (menu[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            selectedItemIndex = i;
            return true;
        }
    }
    return false;
}

void Menu::updateScores(const std::vector<std::string>& scores) {
    for (int i = 0; i < 3; ++i) {
        if (i < scores.size()) {
            scoreText[i].setString(scores[i]);
        } else {
            scoreText[i].setString("");
        }
    }
}

int Menu::getPressedItem() {
    return selectedItemIndex;
}

void Menu::updateFont(const sf::Font& newFont) {
    font = newFont;
    for (int i = 0; i < 4; i++) {
        menu[i].setFont(font);
    }
    scoreTitleText.setFont(font);
    for (int i = 0; i < 3; i++) {
        scoreText[i].setFont(font);
    }
}

