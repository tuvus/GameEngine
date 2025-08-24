#include "game_scene.h"

#include "unit.h"

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
    vector<Vector2> positions = vector<Vector2>();
    positions.emplace_back(rand() % card_game.screen_width, card_game.screen_height);

    while (positions[positions.size() - 1].y > 400) {
        Vector2 prev_pos = positions[positions.size() - 1];
        int new_x = max(
            min((int) prev_pos.x + rand() % 200 - 100, static_cast<int>(card_game.screen_width)),
            0);
        positions.emplace_back(new_x, prev_pos.y - (20 + rand() % 20));
    }

    path = new Path(positions);
    game_manager->Add_Object(new Unit(*game_manager, path, 1));
}

void Game_Scene::Update_UI(chrono::milliseconds) {
    Texture arrow = LoadTextureFromImage(LoadImage("resources/Arrow.png"));
    for (const auto* obj : game_manager->Get_All_Objects()) {
        DrawTextureEx(arrow, obj->pos - Vector2(arrow.width / 2, arrow.height / 2), obj->rot, 1,
                      WHITE);
    }
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
