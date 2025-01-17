#include "FontManager.h"
#include <iostream>

FontManager& FontManager::getInstance() {
    static FontManager instance;
    return instance;
}

bool FontManager::loadFont(const std::string& fontPath) {
    if (font.loadFromFile(fontPath)) {
        currentFontPath = fontPath;
        return true;
    }
    std::cerr << "Failed to load font: " << fontPath << '\n';
    return false;
}

sf::Font& FontManager::getFont() {
    return font;
}

const std::string& FontManager::getCurrentFontPath() const {
    return currentFontPath;
}

const std::string FontManager::getFontName() const {
    int pos = currentFontPath.find_last_of("/\\");
    std::string fileName = (pos == std::string::npos) ? currentFontPath : currentFontPath.substr(pos + 1);
    int dotPos = fileName.find_last_of(".");
    if (dotPos != std::string::npos) {
        fileName = fileName.substr(0, dotPos);
    }
    return fileName;
}
