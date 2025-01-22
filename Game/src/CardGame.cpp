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
    } else {
        Network& network = Get_Application().Get_Network();
        ImGui::Text(("State: " + network.Get_Network_State_Str()).c_str());

        if (network.Get_Network_State() == Network::Server_Running) {
            ImGui::Text(("Clients: " + to_string(network.Get_Num_Connected_Clients())).c_str());
        }

        string close_text = "Shutdown Server";
        if (!network.Is_Server()) close_text = "Leave Server";
        if (ImGui::Button(close_text.c_str())) Get_Application().Close_Network();
    }
    if (ImGui::Button("Close Game")) {
        glfwSetWindowShouldClose(&Get_Window(), true);
    }
}

Card_Game::~Card_Game() = default;
