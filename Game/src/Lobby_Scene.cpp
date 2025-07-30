#include "Lobby_Scene.h"

Lobby_Scene::Lobby_Scene(Card_Game& card_game)
    : Scene(card_game), card_game(card_game), player_count(0) {
    auto* root = new EUI_VBox();
    root_elem = root;
    root->pos = {0, 0};
    root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    root->style.vertical_alignment = Alignment::Center;
    root->style.horizontal_alignment = Alignment::Center;
    root->gap = 20;
    status_text = new EUI_Text("Connecting...");
    root->Add_Child(status_text);
    start_button = new EUI_Button("Start Game", [this, &card_game]() {
        card_game.Get_Network()->call_rpc("startgame");
    });
    start_button->style.padding = {10, 20, 10, 20};
    root->Add_Child(start_button);
    start_button->is_visible = false;
    auto* leave_button = new EUI_Button("Leave Room", [this, &card_game]() {
        card_game.Close_Network();
        card_game.set_ui_screen(MENU);
    });
    leave_button->style.padding = {10, 20, 10, 20};
    root->Add_Child(leave_button);
    card_game.Get_Network()->bind_rpc("setplayercount", [this](int new_player_count) {
        player_count = new_player_count;
        return RPC_Manager::Rpc_Validator_Result::VALID;
    });
    card_game.Get_Network()->bind_rpc("startgame", [&card_game]() {
        if (card_game.Get_Network()->Get_Network_State() != Network::Server_Running &&
            card_game.Get_Network()->Get_Network_State() != Network::Client_Connected)
            return RPC_Manager::Rpc_Validator_Result::INVALID;
        card_game.set_ui_screen(GAME);
        return RPC_Manager::VALID;
    });
}

void Lobby_Scene::Update_UI(std::chrono::milliseconds, EUI_Context context) {
    switch (card_game.Get_Network()->Get_Network_State()) {
        case Network::Setting_Up:
            status_text->text = "Setting up server...";
            break;
        case Network::Client_Connecting:
            status_text->text = "Connecting to server...";
            break;
        case Network::Server_Running:
            if (player_count != card_game.Get_Network()->Get_Num_Connected_Clients() + 1)
                card_game.Get_Network()->call_rpc(
                    "setplayercount", card_game.Get_Network()->Get_Num_Connected_Clients() + 1);
            start_button->is_visible = true;
        case Network::Client_Connected:
            status_text->text = "Players: " + to_string(player_count);
            break;
        case Network::Closing:
            status_text->text = "Leaving";
            break;
        case Network::Closed:
            status_text->text = "Server closed";
            break;
    }
}

void Lobby_Scene::Update(std::chrono::milliseconds) {
}
