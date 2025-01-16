#include "CardGame.h"

#include <imgui.h>

using namespace std;

Card_Game::Card_Game(Application& application) : ApplicationWindow(application) {}

void Card_Game::Render(std::chrono::milliseconds deltaTime) {
    if (&Get_Application().Get_Network() == nullptr) {
        if (ImGui::Button("Start Client")) {
            Get_Application().Start_Client();
        }
        if (ImGui::Button("Start Server")) {
            Get_Application().Start_Server();
        }
    }
    if (ImGui::Button("Close Game")) {
        glfwSetWindowShouldClose(&Get_Window(), true);
    }
}

Card_Game::~Card_Game() = default;
