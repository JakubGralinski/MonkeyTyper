#include <SFML/Graphics.hpp>
#include <fmt/core.h>
#include <iostream>
#include <vector>
#include <cstdlib>
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
    Options,
    Paused,
    FileNameInput
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
        scoreFile << scoreData.playerName << " " << scoreData.score << '\n';
        scoreFile.close();
    } else {
        std::cerr << "Failed to open score file for writing." << '\n';
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
        std::cerr << "Failed to open score file for reading." << '\n';
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
    for (int i = 0; i < 3 && i < scores.size(); ++i) {
        topScores.push_back(scores[i].playerName + ": " + std::to_string(scores[i].score));
    }
    menu.updateScores(topScores);
}

void saveGameState(const std::string& fileName, int gameState, int score, int lives, const std::string& currentWord, const std::string& playerName, const std::vector<MovingWord>& words) {
    json gameStateJson;
    gameStateJson["gameState"] = gameState;
    gameStateJson["score"] = score;
    gameStateJson["lives"] = lives;
    gameStateJson["currentWord"] = currentWord;
    gameStateJson["playerName"] = playerName;

    for (const auto& word : words) {
        json wordJson;
        wordJson["text"] = word.text.getString();
        wordJson["x"] = word.x;
        wordJson["y"] = word.y;
        wordJson["speed"] = word.speed;
        gameStateJson["words"].push_back(wordJson);
    }

    std::ofstream saveFile(fileName);
    if (saveFile.is_open()) {
        saveFile << gameStateJson.dump(4);
        saveFile.close();
    } else {
        std::cerr << "Failed to open save file for writing." << '\n';
    }
}

bool loadGameState(const std::string& fileName, int& gameState, int& score, int& lives, std::string& currentWord, std::string& playerName, std::vector<MovingWord>& words) {
    std::ifstream loadFile(fileName);
    if (!loadFile.is_open()) {
        std::cerr << "Failed to open save file for reading." << '\n';
        return false;
    }

    json gameStateJson;
    loadFile >> gameStateJson;
    loadFile.close();

    gameState = gameStateJson["gameState"];
    score = gameStateJson["score"];
    lives = gameStateJson["lives"];
    currentWord = gameStateJson["currentWord"];
    playerName = gameStateJson["playerName"];

    sf::Font& font = FontManager::getInstance().getFont();

    words.clear();
    for (const auto& wordJson : gameStateJson["words"]) {
        MovingWord newWord;
        newWord.text.setFont(font);
        newWord.text.setCharacterSize(24);
        newWord.text.setFillColor(sf::Color::White);
        newWord.text.setString(wordJson["text"].get<std::string>());
        newWord.x = wordJson["x"];
        newWord.y = wordJson["y"];
        newWord.speed = wordJson["speed"];
        words.push_back(newWord);
    }

    return true;
}

auto main() -> int {

    std::ifstream configFile("/Users/jakubgralinski/CLionProjects/MonkeyTyper/config.json");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config file." <<'\n';
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
    //Using ChatGPT Create a vector of std::string as a dictionary of cs words
    //Sure, here's how you can create a vector of std::string in C++ that serves as a dictionary of computer science (CS) words:#include <iostream>
    //#include <vector>
    //#include <string>
    //
    //int main() {
    //    std::vector<std::string> csDictionary = {
   /* "algorithm", "array", "binary", "boolean", "bug", "byte", "cache", "class",
            "compiler", "database", "debug", "encryption", "error", "function", "hash",
            "integer", "interface", "loop", "memory", "object", "operator", "pointer",
            "protocol", "query", "recursion", "runtime", "script", "stack", "syntax",
            "thread", "variable", "vector", "virtual", "web", "server", "client", "router",
            "firewall", "packet", "SQL", "Python", "Java", "JavaScript", "HTML", "CSS",
            "API", "framework", "password", "login", "logout", "network", "login", "cloud",
            "data", "debugger", "exception", "compile", "build", "deploy", "update",
            "install", "download", "upload", "connect", "disconnect", "binary", "decimal",
            "hexadecimal", "link", "file", "folder", "directory", "path", "search",
            "filter", "sort", "order", "open", "close", "read", "write", "execute",
            "admin", "user", "group", "permission", "access", "backup", "restore",
            "recover", "monitor", "control", "status", "report", "log", "entry", "exit",
            "start", "stop", "pause", "resume", "restart", "shutdown", "reboot", "timeout",
            "retry", "success", "failure", "error", "warning", "alert", "notification",
            "message", "chat", "email", "address", "port", "domain", "website", "url",
            "http", "https", "ftp", "ssh", "telnet", "ping", "traceroute", "request",
            "response", "header", "cookie", "session", "token", "authentication",
            "authorization", "security", "firewall", "proxy", "vpn", "encryption",
            "decryption", "hashing", "checksum", "certificate", "key", "public", "private",
            "signature", "verify", "validation", "verification", "data", "info", "information",
            "metadata", "schema", "table", "row", "column", "field", "record", "index"*/
    //    };

    //
    //    // Output the dictionary
    //    for (const auto& word : csDictionary) {
    //        std::cout << word << std::endl;
    //    }
    //
    //    return 0;
    //}This code snippet creates a vector of std::string with some common CS words and then prints each word to the console. You can add more words to the vector as needed.
    //Do the same but with fruits
    //#include <iostream>
    //#include <vector>
    //#include <string>
    //
    //int main() {
    //    std::vector<std::string> fruits = {
    //        "Apple",
    //        "Banana",
    //        "Cherry",
    //        "Date",
    //        "Elderberry",
    //        "Fig",
    //        "Grape",
    //        "Honeydew",
    //        "Indian Fig",
    //        "Jackfruit",
    //        "Kiwi",
    //        "Lemon",
    //        "Mango",
    //        "Nectarine",
    //        "Orange",
    //        "Papaya",
    //        "Quince",
    //        "Raspberry",
    //        "Strawberry",
    //        "Tangerine",
    //        "Ugli Fruit",
    //        "Vine Peach",
    //        "Watermelon",
    //        "Xigua",
    //        "Yellow Passion Fruit",
    //        "Zucchini" // Note: Botanically, zucchini is a fruit, though commonly considered a vegetable
    //    };
    //
    //    // Output the fruits
    //    for (const auto& fruit : fruits) {
    //        std::cout << fruit << std::endl;
    //    }
    //
    //    return 0;
    //} This program creates a vector named fruits containing names of various fruits and prints each fruit to the console.
    std::vector<std::string> csDictionary = config["dictionaries"]["Computer Science"].get<std::vector<std::string>>();
    std::string mainFont = config["fonts"][0];

    FontManager::getInstance().loadFont(mainFont);
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Monkey Typer");

    int gameState = ShowingMenu;

    Menu menu(window.getSize().x);
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
    namePrompt.setPosition(screenWidth / 2.0f - namePrompt.getGlobalBounds().width *1.35, screenHeight / 3.0f);

    sf::Text nameInput;
    nameInput.setFont(font);
    nameInput.setCharacterSize(24);
    nameInput.setPosition(screenWidth / 2.0f, screenHeight / 2.0f);
    sf::Text instructionText;
    instructionText.setFont(font);
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(50, 50);

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(screenWidth/2, screenHeight/3);

    sf::Text resumeText;
    resumeText.setFont(font);
    resumeText.setCharacterSize(40);
    resumeText.setString("Resume");
    resumeText.setOrigin(resumeText.getLocalBounds().width / 2, resumeText.getLocalBounds().height / 2);
    resumeText.setPosition(screenWidth / 2.7f, screenHeight / 2.0f + 100);

    sf::RectangleShape resumeButton(sf::Vector2f(resumeText.getLocalBounds().width + 20, resumeText.getLocalBounds().height + 20));
    resumeButton.setPosition(resumeText.getPosition().x, resumeText.getPosition().y);
    resumeButton.setFillColor(sf::Color::Transparent);
    resumeButton.setOutlineColor(sf::Color::White);
    resumeButton.setOutlineThickness(2);

    sf::Text pauseText;
    pauseText.setFont(font);
    pauseText.setCharacterSize(40);
    pauseText.setString("PAUSE");
    pauseText.setFillColor(sf::Color::White);
    pauseText.setOrigin(pauseText.getLocalBounds().width / 2, pauseText.getLocalBounds().height / 2);
    pauseText.setPosition(screenWidth / 2.7f, screenHeight / 2.0f - 300.f);

    sf::Text infoPauseText;
    infoPauseText.setFont(font);
    infoPauseText.setCharacterSize(30);
    infoPauseText.setString("Click RESUME to continue or ESC to show Menu. \n\tWarning! Game won't be saved!");
    infoPauseText.setFillColor(sf::Color::Magenta);
    infoPauseText.setOrigin(infoPauseText.getLocalBounds().width / 2, infoPauseText.getLocalBounds().height / 2);
    infoPauseText.setPosition(screenWidth / 2.7f, screenHeight / 2.0f - 100.f);

    sf::RectangleShape pauseOverlay(sf::Vector2f(screenWidth, screenHeight));
    auto pauseColorJson = config["pauseBackgroundColor"];
    auto pauseColor = sf::Color(pauseColorJson[0], pauseColorJson[1], pauseColorJson[2], pauseColorJson[3]);
    pauseOverlay.setFillColor(pauseColor);

    bool resumeButtonHovered = false;

    std::string inputString;
    bool isLoading = false;

    int score = 0;
    int lives = 3;
    std::string currentWord;
    std::string playerName;

    sf::Clock clock;
    float elapsedTime = 0.0f;
    float timeSinceLastSpeedIncrease = 0.0f;

    std::vector<MovingWord> words;

    sf::Vector2f mousePos;

    auto hoverColorJson = config["menuButtonsHoverColor"];
    sf::Color hoverColor(hoverColorJson[0], hoverColorJson[1], hoverColorJson[2], hoverColorJson[3]);

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
                            sf::Vector2i mousePosInt = sf::Mouse::getPosition(window);
                            if (menu.isMouseOver(mousePosInt)) {
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
                            if (optionsMenu.GetPressedItem() == 4) {
                                gameState = ShowingMenu;
                            } else if (optionsMenu.isMouseOverButton(window, optionsMenu.buttons[optionsMenu.GetPressedItem()])) {
                                optionsMenu.HandleClick();
                            }
                        }
                    }
                    break;

                case Playing:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                        gameState = Paused;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                            instructionText.setString("Enter a file name to load:");
                            inputString.clear();
                            inputText.setPosition(50,80);
                            inputText.setString(inputString);
                            isLoading = true;
                            gameState = FileNameInput;
                        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                            instructionText.setString("Enter a file name to save:");
                            inputString.clear();
                            inputText.setPosition(50,80);
                            inputText.setString(inputString);
                            isLoading = false;
                            gameState = FileNameInput;
                        }
                    }
                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode < 128) {
                            char c = event.text.unicode;
                            if (c == '\b') {
                                if (!currentWord.empty()) {
                                    currentWord.pop_back();
                                    currentWordDisplay.setString(currentWord);
                                }
                            } else  if(c != '\r' || c != '\n') {
                                currentWord += c;
                                currentWordDisplay.setString(currentWord);
                            }
                        }
                    }
                    break;

                case Paused:
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                        gameState = ShowingMenu;
                    } else if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                            if (resumeButton.getGlobalBounds().contains(mousePos)) {
                                gameState = Playing;
                            }
                        }
                    }
                    break;

                case FileNameInput:
                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode < 128) {
                            char c = event.text.unicode;
                            if (c == '\b') {
                                if (!inputString.empty()) {
                                    inputString.pop_back();
                                    inputText.setString(inputString);
                                }
                            } else if (c == '\r' || c == '\n') {
                                if (!inputString.empty()) {
                                    if (isLoading) {
                                        if (loadGameState(inputString, gameState, score, lives, currentWord, playerName, words)) {
                                            std::cout << "Game state loaded successfully." << '\n';
                                            scoreboard.setString("SCORE: " + std::to_string(score));
                                            livesDisplay.setString("LIVES: " + std::to_string(lives));
                                        } else {
                                            std::cout << "Failed to load game state." << '\n';
                                        }
                                    } else {
                                        saveGameState(inputString, gameState, score, lives, currentWord, playerName, words);
                                        std::cout << "Game state saved successfully." << '\n';
                                    }
                                    gameState = Playing;
                                }
                            } else{
                                inputString += c;
                                inputText.setString(inputString);
                            }
                        }
                    }
                    break;

                case GameOver:
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

            case Paused:
                window.draw(pauseOverlay);
                window.draw(resumeText);
                window.draw(pauseText);
                window.draw(infoPauseText);

                mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                if (resumeText.getGlobalBounds().contains(mousePos)) {
                    if (!resumeButtonHovered) {
                        resumeButtonHovered = true;
                        resumeText.setFillColor(hoverColor);
                    }
                } else {
                    if (resumeButtonHovered) {
                        resumeButtonHovered = false;
                        resumeText.setFillColor(sf::Color::White);
                    }
                }

                window.display();
                break;

            case FileNameInput:
                window.clear();
                window.draw(pauseOverlay);
                window.draw(instructionText);
                window.draw(inputText);
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
}
