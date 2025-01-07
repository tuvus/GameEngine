#include "CardGame.h"

#include <imgui.h>
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
    if (ImGui::Button("Close Game")) {
        glfwSetWindowShouldClose(&GetWindow(), true);
    }
}

void CardGame::EndGame() {
    Game::EndGame();
    cout << "Ending Game";
}

CardGame::~CardGame() = default;


