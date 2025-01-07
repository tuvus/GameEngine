#include "CardGame.h"

#include <iostream>

using namespace std;

string CardGame::GetName() {
    return "NewCardGame";
}

void CardGame::StartGame() {
    Game::StartGame();
    cout << "Here is the card game!" << endl;
}

void CardGame::UpdateGame(chrono::milliseconds deltaTime) {
    // cout << "DeltaTime: " << deltaTime << endl;
}

void CardGame::EndGame() {
    Game::EndGame();
    cout << "Ending Game";
}

CardGame::~CardGame() = default;


