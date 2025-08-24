#include "game_scene.h"

#include "test_object.h"

Game_Scene::Game_Scene(Card_Game& card_game) : Scene(card_game), card_game(card_game) {
    EUI_HBox* root = new EUI_HBox();
    root_elem = root;

    root->pos = {0, 0};
    root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    root->style.vertical_alignment = Alignment::Center;
    root->style.horizontal_alignment = Alignment::Center;

    step_text = new EUI_Text("Steps: 0");
    root->Add_Child(step_text);
    auto* button = new EUI_Button("Menu", [&card_game] {
        card_game.Close_Network();
        card_game.set_ui_screen(MENU);
    });
    button->style.padding = {10, 20, 10, 20};
    root->Add_Child(button);
    card_game.Get_Network()->connection_events->emplace(
        static_cast<Network_Events_Receiver*>(this));
}

Game_Scene::~Game_Scene() {
    if (card_game.Get_Network() != nullptr)
        card_game.Get_Network()->connection_events->erase(
            static_cast<Network_Events_Receiver*>(this));
}

void Game_Scene::Setup_Scene(unordered_map<Client_ID, Player_ID>* clients_players,
                             Player_ID player_id) {
    game_manager = std::make_unique<Game_Manager>(card_game, *card_game.Get_Network(),
                                                  clients_players, player_id);
    game_manager->Add_Object(new Test_Object(*game_manager, Vector2(0, 0), 0));
}

void Game_Scene::Update_UI(chrono::milliseconds) {
    root_elem->Render();
    step_text->Set_Text("Steps: " + to_string(game_manager->Get_Current_Step()));
}

void Game_Scene::On_Disconnected() {
    card_game.set_ui_screen(MENU);
    card_game.Close_Network();
}

void Game_Scene::On_Server_Stop() {
    card_game.set_ui_screen(MENU);
    card_game.Close_Network();
}
