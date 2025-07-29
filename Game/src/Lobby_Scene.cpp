#include "Lobby_Scene.h"

Lobby_Scene::Lobby_Scene(Card_Game& card_game) : Scene(card_game), card_game(card_game) {
    auto* root = new EUI_VBox();
    root_elem = root;
    root->pos = {0, 0};
    root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    root->style.vertical_alignment = Alignment::Center;
    root->style.horizontal_alignment = Alignment::Center;
    root->gap = 20;
    status_text = new EUI_Text("Connecting...");
    root->Add_Child(status_text);
    auto* leave_button = new EUI_Button("Leave Room", [this, &card_game]() {
        card_game.Close_Network();
        card_game.set_ui_screen(MENU);
    });
    root->Add_Child(leave_button);
}

void Lobby_Scene::Update_UI(std::chrono::milliseconds, EUI_Context context) {
    if (card_game.Get_Network() == nullptr) {
        status_text->text = "Setting up";
    } else {
        switch (card_game.Get_Network()->Get_Network_State()) {
            case Network::Setting_Up:
                status_text->text = "Setting up server...";
                break;
            case Network::Client_Connecting:
                status_text->text = "Connecting to server...";
                break;
            case Network::Server_Running:
            case Network::Client_Connected:
                status_text->text = "Players: " + to_string(card_game.Get_Network()->Get_Num_Connected_Clients() + 1);
                break;
            case Network::Closing:
                status_text->text = "Leaving";
                break;
            case Network::Closed:
                status_text->text = "Server closed";
                break;
        }
    }
}

void Lobby_Scene::Update(std::chrono::milliseconds) {
}
