#include "Lobby_Scene.h"

#include "Game_Scene.h"

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
        status_text->Set_Text("Setting up server...");
    root->Add_Child(status_text);
    start_button = new EUI_Button("Start Game", [this, &card_game]() {
        card_game.Get_Network()->call_rpc(true, "startgame");
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
        status_text->Set_Text("Players: " + to_string(player_count));
        return RPC_Manager::Rpc_Validator_Result::VALID_CALL_ON_CLIENTS;
    });
    card_game.Get_Network()->bind_rpc(
        "addplayer", [this](Client_ID client_id, Player_ID player_id) {
            client_id_to_player_id->emplace(client_id, player_id);
            return RPC_Manager::Rpc_Validator_Result::VALID_CALL_ON_CLIENTS;
        });
    card_game.Get_Network()->bind_rpc("removeplayer", [this](Client_ID client_id) {
        if (client_id_to_player_id->contains(client_id))
            return RPC_Manager::INVALID;
        client_id_to_player_id->erase(client_id);
        return RPC_Manager::Rpc_Validator_Result::VALID_CALL_ON_CLIENTS;
    });
    card_game.Get_Network()->bind_rpc("startgame", [this, &card_game]() {
        if (card_game.Get_Network()->Get_Network_State() != Network::Server_Running &&
            card_game.Get_Network()->Get_Network_State() != Network::Client_Connected)
            return RPC_Manager::Rpc_Validator_Result::INVALID;
        Start_Game();
        return RPC_Manager::VALID_CALL_ON_CLIENTS;
    });
    card_game.Get_Network()->connection_events->emplace(
        static_cast<Network_Events_Receiver*>(this));
    client_id_to_player_id = new unordered_map<Client_ID, Player_ID>();
}

Lobby_Scene::~Lobby_Scene() {
    if (card_game.Get_Network() != nullptr)
        card_game.Get_Network()->connection_events->erase(
            static_cast<Network_Events_Receiver*>(this));
}

void Lobby_Scene::Start_Game() {
    card_game.set_ui_screen(GAME);
    Game_Scene* game_scene = static_cast<Game_Scene*>(card_game.scene);
    game_scene->Setup_Scene(client_id_to_player_id);
}

void Lobby_Scene::Update_UI(std::chrono::milliseconds) {
    root_elem->Render();
}

void Lobby_Scene::Update(std::chrono::milliseconds) {
}

void Lobby_Scene::On_Connected() {
    if (player_count == 0)
        status_text->Set_Text("Connected");
}

void Lobby_Scene::On_Disconnected() {
    card_game.set_ui_screen(MENU);
}

void Lobby_Scene::On_Server_Start() {
    player_count = 1;
    card_game.Get_Network()->call_rpc(true, "setplayercount", player_count);
    card_game.Get_Network()->call_rpc(true, "addplayer", 0, player_id_count++);
}

void Lobby_Scene::On_Server_Stop() {
    card_game.set_ui_screen(MENU);
}

void Lobby_Scene::On_Client_Connected(Client_ID client_id) {
    player_count++;
    card_game.Get_Network()->call_rpc(true, "setplayercount", player_count);
    card_game.Get_Network()->call_rpc(true, "addplayer", client_id, player_id_count++);
    for (const auto& client : *client_id_to_player_id) {
        if (client.first == client_id)
            return;
        card_game.Get_Network()->call_rpc_on_client(client_id, true, "addplayer", client_id,
                                                    player_id_count++);
    }
    if (player_count > 1)
        start_button->is_visible = true;
}

void Lobby_Scene::On_Client_Disconnected(Client_ID client_id) {
    client_id_to_player_id->erase(client_id);
    player_count--;
    card_game.Get_Network()->call_rpc(true, "setplayercount", player_count);
    card_game.Get_Network()->call_rpc(true, "removeplayer", client_id);
    if (player_count <= 1)
        start_button->is_visible = false;
}
