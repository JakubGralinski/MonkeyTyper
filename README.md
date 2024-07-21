# Monkey Typer

Monkey Typer is a typing game developed using C++ and the SFML library. The game involves typing randomly generated words to score points while managing a limited number of lives. The project includes multiple features like a menu, options for customization, saving and loading game states, and maintaining high scores.

## Features

- **Typing Gameplay**: Type words as they appear on the screen to earn points.
- **Multiple Game States**: Menu, Options, Playing, Paused, Game Over, and File Name Input.
- **High Score Management**: Save and display the top scores.
- **Customization Options**: Change font, word speed, dictionary, and word size.
- **Save and Load**: Save and load game states to continue later.

## Getting Started

### Prerequisites

- **C++ Compiler**: A C++17 compatible compiler (e.g., GCC, Clang, MSVC).
- **SFML**: The Simple and Fast Multimedia Library (SFML) for graphics, window, and system modules.
- **FMT**: A formatting library for C++ (libfmt).
- **nlohmann/json**: JSON library for C++.
- **CMake**: Build system to configure and generate build files.

### Installation

1. **Clone the Repository**:

    ```sh
    git clone https://github.com/yourusername/monkey-typer.git
    cd monkey-typer
    ```

2. **Install Dependencies**:
   Follow the instructions for installing SFML, FMT, and nlohmann/json for your platform.

3. **Build the Project**:

    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

4. **Run the Game**:

    ```sh
    ./MonkeyTyper
    ```

### Configuration

The game is configurable via a `config.json` file located in the project directory. The configuration file allows customization of various game parameters:

- Screen resolution
- Maximum words on screen
- Word spawn interval
- Speed increment interval
- Word speed
- Dictionaries (e.g., Computer Science, Fruits)
- Fonts
- Menu button hover colors
- Pause overlay background color

### Example `config.json`

```json
{
    "resolution": {
        "width": 800,
        "height": 600
    },
    "maxWords": 10,
    "spawnInterval": 1.0,
    "speedIncrementInterval": 30.0,
    "wordSpeed": 1.0,
    "dictionaries": {
        "Computer Science": ["algorithm", "array", "binary", ...],
        "Fruits": ["Apple", "Banana", "Cherry", ...]
    },
    "fonts": ["fonts/FiraCode-Bold.ttf", "fonts/AnotherFont.ttf"],
    "menuButtonsHoverColor": [255, 255, 255, 200],
    "pauseBackgroundColor": [0, 0, 0, 150]
}
