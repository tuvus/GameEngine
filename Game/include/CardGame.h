#pragma once
#include "Game.h"

using namespace std;

class CardGame : public Game {
public:
    string GetName() override;
    void StartGame() override;
    void UpdateGame(chrono::milliseconds deltaTime) override;
    void EndGame() override;
    ~CardGame() override;
};
