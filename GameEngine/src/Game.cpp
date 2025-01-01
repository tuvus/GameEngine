#include "Game.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

void CreateGame(Game& game) {
    cout << "Creating game: " + game.GetName() << endl << endl;
    game.StartGame();
}

Game::Game() = default;

void Game::StartGame() {
    cout << "Starting game: " + GetName() << endl;
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

Game::~Game() = default;

