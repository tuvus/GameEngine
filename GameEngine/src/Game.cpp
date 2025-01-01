#include "Game.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <GLFW/glfw3.h>
using namespace std;

void CreateGame(Game& game) {
    cout << "Creating game: " + game.GetName() << endl << endl;
    game.StartGame();
}

Game::Game() = default;

void Game::StartGame() {
    cout << "Starting game: " + GetName() << endl;
    if (!glfwInit()) {
        cerr << "An error occurred with initializing glfw!" << endl;
        exit(-1);
    }

    auto errorCallBack = [](const int error, const char* description) {
        cerr << "glfw error " << error << " " << description;
    };
    glfwSetErrorCallback(errorCallBack);
    GLFWwindow* window = glfwCreateWindow(1280,800, GetName().c_str(), nullptr, nullptr);
    if (!window) {
        cerr << "Opening window failed" << endl;
        exit(-1);
    }
    GameLoop();
}


void Game::GameLoop() {
    chrono::time_point<chrono::system_clock> frameEndTime = chrono::system_clock::now();
    for (int i = 0; i < 100; ++i) {
        chrono::time_point<chrono::system_clock> frameStartTime = chrono::system_clock::now();
        UpdateGame(chrono::duration_cast<std::chrono::milliseconds>(frameStartTime - frameEndTime));
        this_thread::sleep_until(frameStartTime + 16ms);
        frameEndTime = chrono::system_clock::now();
    }
    EndGame();
}

void Game::EndGame() {
    glfwTerminate();
}

Game::~Game() = default;
