cmake_minimum_required(VERSION 3.0)
project(MonkeyTyper)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)

# Include FetchContent module to download fmt and json libraries
include(FetchContent)

# Fetch and make fmt library available
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.2.1
)
FetchContent_MakeAvailable(fmt)

# Fetch and make nlohmann/json library available
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# Add the executable
add_executable(MonkeyTyper main.cpp Menu.cpp Menu.h)

# Link SFML libraries to your executable
target_link_libraries(MonkeyTyper sfml-graphics sfml-window sfml-system)

# Link fmt library to your executable
target_link_libraries(MonkeyTyper fmt)

# Link json library to your executable
target_link_libraries(MonkeyTyper nlohmann_json::nlohmann_json)
