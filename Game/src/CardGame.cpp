#include "CardGame.h"

#include <imgui.h>

using namespace std;

Card_Game::Card_Game(Application& application) : ApplicationWindow(application) {}

void Card_Game::Render(std::chrono::milliseconds deltaTime) {
    if (ImGui::Button("Close Game")) {
        glfwSetWindowShouldClose(&Get_Window(), true);
    }
}

Card_Game::~Card_Game() = default;
