#include "CardGame.h"

#include <imgui.h>
#include <iostream>

using namespace std;

string Card_Game::Get_Name() {
    return "NewCardGame";
}

void Card_Game::Start_Game() {
    Game::Start_Game();
    cout << "Here is the card game!" << endl;
}

void Card_Game::Update_Game(chrono::milliseconds deltaTime) {
    if (ImGui::Button("Close Game")) {
        glfwSetWindowShouldClose(&GetWindow(), true);
    }
}

void Card_Game::End_Game() {
    Game::End_Game();
    cout << "Ending Game";
}

Card_Game::~Card_Game() = default;


