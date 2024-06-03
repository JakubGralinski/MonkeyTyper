#include "OptionsMenu.h"
#include "FontManager.h"
#include <fstream>
#include <iostream>

OptionsMenu::OptionsMenu(float width, float height, Menu& menu)
        : menu(menu) {
    font = FontManager::getInstance().getFont();

    std::ifstream configFile("/Users/jakubgralinski/CLionProjects/MonkeyTyper/config.json");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config file." << std::endl;
        return;
    }

    nlohmann::json config;
    configFile >> config;
    configFile.close();

    auto hoverColorJson = config["menuButtonsHoverColor"];
    sf::Color hoverColor(hoverColorJson[0], hoverColorJson[1], hoverColorJson[2], hoverColorJson[3]);

    for (const auto& fontPath : config["fonts"]) {
        availableFonts.push_back(fontPath.get<std::string>());
    }
    currentFontIndex = 0;

    for (const auto& dictionary : config["dictionaries"].items()) {
        availableDictionaries.push_back(dictionary.key());
    }
    currentDictionaryIndex = 0;

    float buttonPadding = config["buttonPadding"];
    float buttonHeight = 40.0f; // Assuming a fixed height for simplicity
    float totalHeight = (buttonHeight + buttonPadding) * 4 - buttonPadding;
    float buttonInitialYPosition = (height - totalHeight) / 2.0f;

    std::string buttonLabels[] = {
            "CURRENT FONT: " + extractFontName(availableFonts[currentFontIndex]),
            "SPEED: " + speedOptions[currentSpeedIndex],
            "DICTIONARY: " + availableDictionaries[currentDictionaryIndex],
            "BACK"
    };

    for (int i = 0; i < 4; ++i) {
        buttons[i].setFont(font);
        buttons[i].setString(buttonLabels[i]);
        buttons[i].setCharacterSize(buttonHeight);
        buttons[i].setFillColor(sf::Color::White);
        buttons[i].setOrigin(buttons[i].getLocalBounds().width / 2, buttons[i].getLocalBounds().height / 2);
        buttons[i].setPosition(sf::Vector2f(width / 2, buttonInitialYPosition + (i * (buttonHeight + buttonPadding))));

        hoverStates[i].isHovered = false;
        hoverStates[i].normalColor = sf::Color::White;
        hoverStates[i].hoverColor = hoverColor;
    }

    selectedItemIndex = -1;
}

std::string OptionsMenu::extractFontName(const std::string& fontPath) {
    int pos = fontPath.find_last_of("/\\");
    std::string fileName = (pos == std::string::npos) ? fontPath : fontPath.substr(pos + 1);
    int dotPos = fileName.find_last_of(".");
    if (dotPos != std::string::npos) {
        fileName = fileName.substr(0, dotPos);
    }
    return fileName;
}

void OptionsMenu::draw(sf::RenderWindow &window) {
    for (int i = 0; i < 4; ++i) {
        if (isMouseOverButton(window, buttons[i])) {
            buttons[i].setFillColor(hoverStates[i].hoverColor);
            selectedItemIndex = i;
        } else {
            buttons[i].setFillColor(hoverStates[i].normalColor);
        }
        window.draw(buttons[i]);
    }
}

bool OptionsMenu::isMouseOverButton(sf::RenderWindow &window, sf::Text &button) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect buttonBounds = button.getGlobalBounds();
    return buttonBounds.contains(static_cast<sf::Vector2f>(mousePos));
}

int OptionsMenu::GetPressedItem() {
    return selectedItemIndex;
}

void OptionsMenu::HandleClick() {
    if (selectedItemIndex < 0 || selectedItemIndex >= 4) {
        return; // Invalid selection index
    }

    switch (selectedItemIndex) {
        case 0:
            currentFontIndex = (currentFontIndex + 1) % availableFonts.size();
            if (FontManager::getInstance().loadFont(availableFonts[currentFontIndex])) {
                font = FontManager::getInstance().getFont();
                buttons[0].setString("CURRENT FONT: " + extractFontName(availableFonts[currentFontIndex]));
                for (int i = 0; i < 4; ++i) {
                    buttons[i].setFont(font);
                }
                menu.updateFont(font); // Update the font in the main menu
            }
            break;
        case 1:
            currentSpeedIndex = (currentSpeedIndex + 1) % 3;
            buttons[1].setString("SPEED: " + speedOptions[currentSpeedIndex]);
            break;
        case 2:
            currentDictionaryIndex = (currentDictionaryIndex + 1) % availableDictionaries.size();
            buttons[2].setString("DICTIONARY: " + availableDictionaries[currentDictionaryIndex]);
            break;
        default:
            std::cerr << "Error" << std::endl;
    }
}
