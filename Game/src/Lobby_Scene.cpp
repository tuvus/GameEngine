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
    if (card_game.Get_Network()->Is_Server())
        status_text->text = "Setting up server...";
    root->Add_Child(status_text);
    start_button = new EUI_Button(
        "Start Game", [this, &card_game]() { card_game.Get_Network()->call_rpc("startgame"); });
    start_button->style.padding = {10, 20, 10, 20};
    root->Add_Child(start_button);
    start_button->is_visible = false;
    auto* leave_button = new EUI_Button("Leave Room", [this, &card_game]() {
        card_game.set_ui_screen(MENU);
        card_game.Close_Network();
    });
    leave_button->style.padding = {10, 20, 10, 20};
    root->Add_Child(leave_button);
    card_game.Get_Network()->bind_rpc("setplayercount", [this](int new_player_count) {
        player_count = new_player_count;
        status_text->text = "Players: " + to_string(player_count);
        return RPC_Manager::Rpc_Validator_Result::VALID_CALL_ON_CLIENTS;
    });
    card_game.Get_Network()->bind_rpc("startgame", [&card_game]() {
        if (card_game.Get_Network()->Get_Network_State() != Network::Server_Running &&
            card_game.Get_Network()->Get_Network_State() != Network::Client_Connected)
            return RPC_Manager::Rpc_Validator_Result::INVALID;
        card_game.set_ui_screen(GAME);
        return RPC_Manager::VALID_CALL_ON_CLIENTS;
    });
    card_game.Get_Network()->connection_events->emplace(static_cast<Network_Events_Receiver*>(this));
}

Lobby_Scene::~Lobby_Scene() {
    card_game.Get_Network()->connection_events->erase(static_cast<Network_Events_Receiver*>(this));
}

void Lobby_Scene::Update_UI(std::chrono::milliseconds, EUI_Context context) {
}

void Lobby_Scene::Update(std::chrono::milliseconds) {
}

void Lobby_Scene::On_Connected() {
    if (player_count == 0)
        status_text->text = "Connected";
}

void Lobby_Scene::On_Disconnected() {
    card_game.set_ui_screen(MENU);
}

void Lobby_Scene::On_Server_Start() {
    player_count = 1;
    card_game.Get_Network()->call_rpc("setplayercount", player_count);
}

void Lobby_Scene::On_Server_Stop() {
    card_game.set_ui_screen(MENU);
}

void Lobby_Scene::On_Client_Connected(Client_ID) {
    player_count++;
    card_game.Get_Network()->call_rpc("setplayercount", player_count);
    if (player_count > 1)
        start_button->is_visible = true;
}

void Lobby_Scene::On_Client_Disconnected(Client_ID) {
    player_count--;
    card_game.Get_Network()->call_rpc("setplayercount", player_count);
    if (player_count <= 1)
        start_button->is_visible = false;
}
