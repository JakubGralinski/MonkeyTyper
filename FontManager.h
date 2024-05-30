// FontManager.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class FontManager {
public:
    static FontManager& getInstance();
    bool loadFont(const std::string& fontPath);
    sf::Font& getFont();
    const std::string& getCurrentFontPath() const;
    const std::string getFontName() const;

private:
    FontManager() = default;
    sf::Font font;
    std::string currentFontPath;
};
