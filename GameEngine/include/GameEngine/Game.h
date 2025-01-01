#pragma once
#include <chrono>

using namespace std;

class Game {
private:
    void GameLoop();

public:
    Game();
    virtual string GetName() = 0;
    virtual void StartGame();
    /**
     * Handles the main game logic and is called every frame.
     * @param deltaTime The game time of this frame in milliseconds
     */
    virtual void UpdateGame(std::chrono::milliseconds deltaTime) = 0;
    virtual void EndGame();
    virtual ~Game();
};

void CreateGame(Game& game);