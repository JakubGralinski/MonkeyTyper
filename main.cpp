#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Menu.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct MovingWord {
    sf::Text text;
    float x, y;
    float speed;
};

enum GameState {
    ShowingMenu,
    Playing,
    GameOver
};

std::string getRandomWord(std::vector<std::string>& wordList) {
    return wordList[rand() % wordList.size()];
}

int main() {
    // Read JSON configuration
    std::ifstream configFile("/Users/jakubgralinski/CLionProjects/MonkeyTyper/config.json");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config file." << std::endl;
        return 1;
    }

    json config;
    configFile >> config;
    configFile.close();

    // Extract configuration parameters from JSON
    int screenWidth = config["resolution"]["width"];
    int screenHeight = config["resolution"]["height"];
    int maxWords = config["maxWords"];
    float spawnInterval = config["spawnInterval"];
    float speedIncrementInterval = config["speedIncrementInterval"];
    std::vector<std::string> csDictionary = config["dictionaries"]["cs"];
    std::string mainFont = config["fonts"][0];
    float wordSpeed = config["wordSpeed"];

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Monkey Typer");
    sf::Font font;
    if (!font.loadFromFile(mainFont)) {
        std::cerr << "Failed to load font." << std::endl;
        return 1;
    }

    sf::Text scoreboard;
    scoreboard.setFont(font);
    scoreboard.setCharacterSize(24);
    scoreboard.setString("SCORE: 0");
    scoreboard.setPosition(10.f, 10.f);

    sf::Text livesDisplay;
    livesDisplay.setFont(font);
    livesDisplay.setCharacterSize(24);
    livesDisplay.setString("LIVES: 3");
    livesDisplay.setPosition(10.f, 40.f);

    sf::Text currentWordDisplay;
    currentWordDisplay.setFont(font);
    currentWordDisplay.setCharacterSize(24);
    currentWordDisplay.setPosition(10.f, window.getSize().y - 40.f);

    GameState gameState = ShowingMenu;
    Menu menu(window.getSize().x, window.getSize().y);

    int score = 0;
    int lives = 3;
    std::string currentWord;

    sf::Clock clock;
    float elapsedTime = 0.0f;
    float timeSinceLastSpeedIncrease = 0.0f;

    std::vector<MovingWord> words;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (gameState == ShowingMenu) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        if (menu.isMouseOver(mousePos)) {
                            if (menu.getPressedItem() == 1) { // Adjusted index for "Play"
                                gameState = Playing;
                            } else if (menu.getPressedItem() == 2) { // Adjusted index for "Options"
                                // Handle "Options" if necessary
                            } else if (menu.getPressedItem() == 3) { // Adjusted index for "Quit"
                                window.close();
                            }
                        }
                    }
                }
            } else if (gameState == Playing) {
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode < 128) {
                        char c = event.text.unicode;
                        if (c == '\b') {
                            if (!currentWord.empty()) {
                                currentWord.pop_back();
                                currentWordDisplay.setString(currentWord);
                            }
                        } else {
                            currentWord += c;
                            currentWordDisplay.setString(currentWord);
                        }
                    }
                }
            }
        }

        if (gameState == ShowingMenu) {
            window.clear();
            menu.draw(window);
            window.display();
            continue;
        }

        if (gameState == Playing) {
            elapsedTime += clock.restart().asSeconds();
            timeSinceLastSpeedIncrease += elapsedTime;

            if (timeSinceLastSpeedIncrease >= speedIncrementInterval) {
                wordSpeed += speedIncrementInterval; // Increase the speed
                timeSinceLastSpeedIncrease = 0.0f;
            }

            if (elapsedTime >= spawnInterval && words.size() < maxWords) {
                int baseSpawnCount = 1;
                int additionalSpawnCount = score / 10;
                int numWordsToSpawn = std::min(maxWords - static_cast<int>(words.size()), baseSpawnCount + additionalSpawnCount);

                for (int i = 0; i < numWordsToSpawn; ++i) {
                    MovingWord newWord;
                    newWord.text.setFont(font);
                    newWord.text.setCharacterSize(24);
                    newWord.text.setFillColor(sf::Color::White);
                    newWord.text.setString(getRandomWord(csDictionary));
                    newWord.speed = wordSpeed;
                    newWord.x = window.getSize().x;
                    newWord.y = rand() % (window.getSize().y - 100);
                    words.push_back(newWord);
                }

                elapsedTime = 0.0f;
            }

            if (!words.empty()) {
                bool wordMatched = false;
                for (auto it = words.begin(); it != words.end(); ++it) {
                    std::string displayedWord = it->text.getString().toAnsiString();
                    if (currentWord == displayedWord) {
                        words.erase(it);
                        score++;
                        scoreboard.setString("SCORE: " + std::to_string(score));
                        wordMatched = true;
                        break;
                    }
                }
                if (wordMatched) {
                    currentWord.clear();
                    currentWordDisplay.setString(currentWord);
                }
            }

            for (auto it = words.begin(); it != words.end();) {
                it->x -= it->speed * 0.05f;
                if (it->x + it->text.getLocalBounds().width < 0) {
                    it = words.erase(it);
                    lives--;
                    livesDisplay.setString("LIVES: " + std::to_string(lives));
                    if (lives <= 0) {
                        gameState = GameOver;
                        break;
                    }
                } else {
                    it->text.setPosition(it->x, it->y);
                    ++it;
                }
            }

            window.clear(sf::Color::Black);
            for (const auto& word : words) {
                window.draw(word.text);
            }
            window.draw(scoreboard);
            window.draw(livesDisplay);
            window.draw(currentWordDisplay);
            window.display();
        } else if (gameState == GameOver) {
            window.clear();
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setCharacterSize(50);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setString("Game Over! Final Score: " + std::to_string(score));
            gameOverText.setPosition(window.getSize().x / 4, window.getSize().y / 2);
            window.draw(gameOverText);
            window.display();
        }
    }

    return 0;
}
