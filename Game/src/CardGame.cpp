#include "CardGame.h"

#include <imgui.h>
#include <iostream>

using namespace std;

string Card_Game::Get_Name() {
    return "NewCardGame";
}

void Card_Game::Start_Application(bool server) {
    Application::Start_Application(server);
    cout << "Here is the card game!" << endl;
}

void Card_Game::Update_Application(chrono::milliseconds deltaTime) {
    if (ImGui::Button("Close Game")) {
        glfwSetWindowShouldClose(&Get_Window(), true);
    }
}

void Card_Game::End_Application() {
    Application::End_Application();
    cout << "Ending Game";
}

Card_Game::~Card_Game() = default;


