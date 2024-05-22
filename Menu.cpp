#include "Menu.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
using json = nlohmann::json;

Menu::Menu(float width, float height) {
    if (!font.loadFromFile("/Users/jakubgralinski/CLionProjects/MonkeyTyper/fonts/FiraCode-Bold.ttf")) {
        // Handle error
    }

    // Read JSON configuration
    std::ifstream configFile("/Users/jakubgralinski/CLionProjects/MonkeyTyper/config.json");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config file." << std::endl;
    } else {
        json config;
        configFile >> config;
        auto hoverColorJson = config["menuButtonsHoverColor"];
        hoverColor = sf::Color(hoverColorJson[0], hoverColorJson[1], hoverColorJson[2], hoverColorJson[3]);
        configFile.close();
    }

    // Title setup
    menu[0].setFont(font);
    menu[0].setFillColor(sf::Color::Yellow);
    menu[0].setString("Monkey Typer");
    menu[0].setCharacterSize(80);
    menu[0].setStyle(sf::Text::Bold);
    menu[0].setOrigin(menu[0].getLocalBounds().width / 2, menu[0].getLocalBounds().height / 2);
    menu[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 0.5f));

    // Menu item setup
    for (int i = 1; i < MAX_NUMBER_OF_ITEMS; i++) {
        menu[i].setFont(font);
        menu[i].setFillColor(sf::Color::White); // Initial color
        menu[i].setCharacterSize(40);
        menu[i].setOrigin(menu[i].getLocalBounds().width / 2, menu[i].getLocalBounds().height / 2);
        menu[i].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * (i + 0.5f)));
    }

    menu[1].setString("Play");
    menu[2].setString("Options");
    menu[3].setString("Quit");

    selectedItemIndex = 1; // Start at the first menu item, not the title

    animationTime = 0.0f;
}

Menu::~Menu() {}

void Menu::draw(sf::RenderWindow &window) {
    // Update title animation
    float deltaTime = clock.restart().asSeconds();
    animationTime += animationSpeed * deltaTime;
    float scale = 1.0f + scaleFactor * std::sin(animationTime);
    menu[0].setScale(scale, scale);
    menu[0].setPosition(window.getSize().x / 2, window.getSize().y / (MAX_NUMBER_OF_ITEMS + 1) * 0.5f);

    // Draw menu items
    for (int i = 1; i < MAX_NUMBER_OF_ITEMS; i++) {
        if (isMouseOver(sf::Mouse::getPosition(window)) && selectedItemIndex == i) {
            // Set hover color for the selected item when mouse is over it
            menu[i].setFillColor(hoverColor);
        } else {
            // Set the default color for other items or when mouse is not over them
            menu[i].setFillColor(sf::Color::White);
        }
        // Update the origin and position to ensure each word is centered
        menu[i].setOrigin(menu[i].getLocalBounds().width / 2, menu[i].getLocalBounds().height / 2);
        menu[i].setPosition(sf::Vector2f(window.getSize().x / 2, menu[i].getPosition().y));

        window.draw(menu[i]);
    }

    // Draw title
    window.draw(menu[0]);
}

void Menu::MoveUp() {
    if (selectedItemIndex - 1 >= 1) { // Avoid selecting the title
        menu[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menu[selectedItemIndex].setFillColor(hoverColor);
    }
}

void Menu::MoveDown() {
    if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS) {
        menu[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menu[selectedItemIndex].setFillColor(hoverColor);
    }
}

bool Menu::isMouseOver(sf::Vector2i mousePos) {
    for (int i = 1; i < MAX_NUMBER_OF_ITEMS; i++) { // Avoid selecting the title
        sf::FloatRect boundingBox = menu[i].getGlobalBounds();
        if (boundingBox.contains(static_cast<sf::Vector2f>(mousePos))) {
            selectedItemIndex = i;
            return true;
        }
    }
    return false;
}
