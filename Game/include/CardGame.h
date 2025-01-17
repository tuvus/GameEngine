#pragma once
#include "Game.h"

using namespace std;

class Card_Game : public Game {
public:
    string Get_Name() override;
    void Start_Game() override;
    void Update_Game(chrono::milliseconds deltaTime) override;
    void End_Game() override;
    ~Card_Game() override;
};
