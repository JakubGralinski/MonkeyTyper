#include "Menu.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Menu::Menu(float width, float height) {
    if (!font.loadFromFile("/Users/jakubgralinski/CLionProjects/MonkeyTyper/fonts/FiraCode-Bold.ttf")) {
        // Handle error
    }

    std::ifstream configFile("/Users/jakubgralinski/CLionProjects/MonkeyTyper/config.json");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config file." << std::endl;
    }
    json config;
    configFile >> config;
    auto hoverColorJson = config["menuButtonsHoverColor"];
    hoverColor = sf::Color(hoverColorJson[0], hoverColorJson[1], hoverColorJson[2], hoverColorJson[3]);
    float buttonPadding = config["buttonPadding"];
    float buttonInitialYPosition = config["buttonInitialYPosition"];
    configFile.close();

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

