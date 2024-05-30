#include <SFML/Graphics.hpp>
#include <fmt/core.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Menu.h"
#include "OptionsMenu.h"
#include "FontManager.h"

using json = nlohmann::json;

struct MovingWord {
    sf::Text text;
    float x, y;
    float speed;
};

enum GameState {
    ShowingMenu,
    EnteringName,
    Playing,
    GameOver,
    Options
};

std::string getRandomWord(std::vector<std::string>& wordList) {
    return wordList[rand() % wordList.size()];
}

struct ScoreData {
    std::string playerName;
    int score;
};

bool compareScores(const ScoreData& a, const ScoreData& b) {
    return a.score > b.score;
}

void saveScore(const ScoreData& scoreData) {
    std::ofstream scoreFile("/Users/jakubgralinski/CLionProjects/MonkeyTyper/scores.txt", std::ios::app);
    if (scoreFile.is_open()) {
        scoreFile << scoreData.playerName << " " << scoreData.score << std::endl;
        scoreFile.close();
    } else {
        std::cerr << "Failed to open score file for writing." << std::endl;
    }
}

std::vector<ScoreData> loadScores() {
    std::vector<ScoreData> scores;
    std::ifstream scoreFile("/Users/jakubgralinski/CLionProjects/MonkeyTyper/scores.txt");
    if (scoreFile.is_open()) {
        std::string playerName;
        int score;
        while (scoreFile >> playerName >> score) {
            scores.push_back({playerName, score});
        }
        scoreFile.close();
    } else {
        std::cerr << "Failed to open score file for reading." << std::endl;
    }
    return scores;
}

void updateScores(const ScoreData& newScore) {
    std::vector<ScoreData> scores = loadScores();
    scores.push_back(newScore);
    std::sort(scores.begin(), scores.end(), compareScores);
    saveScore(newScore);
}

void displayScores(Menu& menu) {
    std::vector<ScoreData> scores = loadScores();
    std::sort(scores.begin(), scores.end(), compareScores);
    std::vector<std::string> topScores;
    for (size_t i = 0; i < 3 && i < scores.size(); ++i) {
        topScores.push_back(scores[i].playerName + ": " + std::to_string(scores[i].score));
    }
    menu.updateScores(topScores);
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::ifstream configFile("/Users/jakubgralinski/CLionProjects/MonkeyTyper/config.json");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config file." << std::endl;
        return 1;
    }

    json config;
    configFile >> config;
    configFile.close();

    int screenWidth = config["resolution"]["width"];
    int screenHeight = config["resolution"]["height"];
    int maxWords = config["maxWords"];
    float spawnInterval = config["spawnInterval"];
    float speedIncrementInterval = config["speedIncrementInterval"];
    float wordSpeed = config["wordSpeed"];
    std::vector<std::string> csDictionary = config["dictionaries"]["Computer Science"].get<std::vector<std::string>>();
    std::string mainFont = config["fonts"][0];

    FontManager::getInstance().loadFont(mainFont);
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Monkey Typer");

    int gameState = ShowingMenu;

    Menu menu(window.getSize().x, window.getSize().y);
    OptionsMenu optionsMenu(window.getSize().x, window.getSize().y, menu);

    sf::Font& font = FontManager::getInstance().getFont();

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

    sf::Text namePrompt;
    namePrompt.setFont(font);
    namePrompt.setCharacterSize(24);
    namePrompt.setString("Enter your nickname:");
    namePrompt.setPosition(screenWidth / 2.0f - namePrompt.getGlobalBounds().width / 2.0f, screenHeight / 2.0f - 30.f);

    sf::Text nameInput;
    nameInput.setFont(font);
    nameInput.setCharacterSize(24);
    nameInput.setPosition(screenWidth / 2.0f, screenHeight / 2.0f);

    int score = 0;
    int lives = 3;
    std::string currentWord;
    std::string playerName;

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

            switch (gameState) {
                case ShowingMenu:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                            if (menu.isMouseOver(mousePos)) {
                                switch (menu.getPressedItem()) {
                                    case 1:
                                        gameState = EnteringName;
                                        score = 0;
                                        lives = 3;
                                        words.clear();
                                        currentWord.clear();
                                        playerName.clear();
                                        scoreboard.setString("SCORE: " + std::to_string(score));
                                        livesDisplay.setString("LIVES: " + std::to_string(lives));
                                        break;
                                    case 2:
                                        gameState = Options;
                                        break;
                                    case 3:
                                        window.close();
                                        break;
                                }
                            }
                        }
                    }
                    break;

                case EnteringName:
                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode < 128) {
                            char c = event.text.unicode;
                            if (c == '\b') {
                                if (!playerName.empty()) {
                                    playerName.pop_back();
                                }
                            } else if (c == '\n' || c == '\r') {
                                if (!playerName.empty()) {
                                    gameState = Playing;
                                }
                            } else {
                                playerName += c;
                            }
                            nameInput.setString(playerName);
                            nameInput.setPosition(window.getSize().x / 2.0f - nameInput.getGlobalBounds().width / 2.0f, window.getSize().y / 2.0f);
                        }
                    }
                    break;

                case Options:
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (optionsMenu.GetPressedItem() == 3) {
                                gameState = ShowingMenu;
                            } else if (optionsMenu.isMouseOverButton(window,
                                                                     optionsMenu.buttons[optionsMenu.GetPressedItem()])) {
                                optionsMenu.HandleClick();
                            }
                        }
                    }
                    break;

                case Playing:
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
                    break;

                case GameOver:
                    // Handle GameOver event if needed
                    break;
            }
        }
        switch (gameState) {
            case ShowingMenu:
                displayScores(menu);
                window.clear();
                menu.draw(window);
                window.display();
                continue;

            case Options:
                window.clear();
                optionsMenu.draw(window);
                window.display();
                continue;

            case EnteringName:
                window.clear();
                window.draw(namePrompt);
                window.draw(nameInput);
                window.display();
                continue;

            case Playing:
                elapsedTime += clock.restart().asSeconds();
                timeSinceLastSpeedIncrease += elapsedTime;

                if (timeSinceLastSpeedIncrease >= speedIncrementInterval) {
                    wordSpeed += speedIncrementInterval;
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
                        std::vector<std::string> dictionary = config["dictionaries"][optionsMenu.availableDictionaries[optionsMenu.currentDictionaryIndex]].get<std::vector<std::string>>();
                        newWord.text.setString(getRandomWord(dictionary));

                        // Adjust the speed based on the selected speed setting
                        if (optionsMenu.speedOptions[optionsMenu.currentSpeedIndex] == "Slow") {
                            newWord.speed = wordSpeed * 1;
                        } else if (optionsMenu.speedOptions[optionsMenu.currentSpeedIndex] == "Normal") {
                            newWord.speed = wordSpeed * 2;
                        } else if (optionsMenu.speedOptions[optionsMenu.currentSpeedIndex] == "Fast") {
                            newWord.speed = wordSpeed * 4.0;
                        }

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
                break;

            case GameOver:
                window.clear();
                ScoreData newScore;
                newScore.playerName = playerName;
                newScore.score = score;

                updateScores(newScore);
                displayScores(menu);
                playerName.clear();
                gameState = ShowingMenu;
                break;
        }
    }

    return 0;
}
