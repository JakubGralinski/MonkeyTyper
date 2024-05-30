#include "OptionsMenu.h"
#include "FontManager.h"
#include <fstream>
#include <iostream>

OptionsMenu::OptionsMenu(float width, float height) {
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

    float buttonPadding = config["buttonPadding"];
    float buttonInitialYPosition = config["buttonInitialYPosition"];
    std::string buttonLabels[] = {
            "CURRENT FONT: " + extractFontName(availableFonts[currentFontIndex]),
            "SPEED: ",
            "DICTIONARY: ",
            "BACK"
    };

    for (int i = 0; i < 4; ++i) {
        buttons[i].setFont(font);
        buttons[i].setString(buttonLabels[i]);
        buttons[i].setCharacterSize(40);
        buttons[i].setFillColor(sf::Color::White);
        buttons[i].setOrigin(buttons[i].getLocalBounds().width / 2, buttons[i].getLocalBounds().height / 2);
        buttons[i].setPosition(sf::Vector2f(width / 2, buttonInitialYPosition + (i * (40 + buttonPadding))));

        hoverStates[i].isHovered = false;
        hoverStates[i].normalColor = sf::Color::White;
        hoverStates[i].hoverColor = hoverColor;
    }

}

std::string OptionsMenu::extractFontName(const std::string& fontPath) {
    size_t pos = fontPath.find_last_of("/\\");
    std::string fileName = (pos == std::string::npos) ? fontPath : fontPath.substr(pos + 1);
    size_t dotPos = fileName.find_last_of(".");
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

    if (selectedItemIndex == 0) { // Fonts button
        currentFontIndex = (currentFontIndex + 1) % availableFonts.size();
        if (FontManager::getInstance().loadFont(availableFonts[currentFontIndex])) {
            font = FontManager::getInstance().getFont();
            buttons[0].setString("CURRENT FONT: " + extractFontName(availableFonts[currentFontIndex]));
            for (int i = 0; i < 4; ++i) {
                buttons[i].setFont(font);
            }
        }
    } else if (selectedItemIndex == 3) { // Back button
        //gameState = showingMenuState;  // Set the gameState to ShowingMenu
    }
}
