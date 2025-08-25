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
                             Player_ID player_id, long seed) {
    game_manager = std::make_unique<Game_Manager>(card_game, *card_game.Get_Network(),
                                                  clients_players, player_id, seed);
    vector<Vector2> positions = vector<Vector2>();
    static uniform_int_distribution<int> start_dist(0, INT_MAX);
    positions.emplace_back(start_dist(game_manager->random) % card_game.screen_width,
                           card_game.screen_height - 10);

    while (positions[positions.size() - 1].y > 80) {
        Vector2 prev_pos = positions[positions.size() - 1];
        static uniform_int_distribution<int> move_dist(-100, 100);
        int new_x = max(min((int) prev_pos.x + move_dist(game_manager->random),
                            static_cast<int>(card_game.screen_width)),
                        0);
        static uniform_int_distribution<int> forward_dist(20, 40);
        positions.emplace_back(new_x, prev_pos.y - forward_dist(game_manager->random));
    }

    path = new Path(positions);
    game_manager->Add_Object(
        new Unit(*game_manager, LoadTextureFromImage(LoadImage("resources/Arrow.png")), path, 1));

    game_ui_manager = make_unique<Game_UI_Manager>(*game_manager);
}

void Game_Scene::Update_UI(chrono::milliseconds delta_time) {
    // Visualize path
    Vector2 past_pos = Vector2One() * -1;
    for (const auto& pos : path->positions) {
        if (past_pos != Vector2One() * -1)
            DrawLineEx(past_pos, pos, 5, BLACK);
        past_pos = pos;
    }

    // Draw arrows
    game_ui_manager->Update_UI(delta_time);

    root_elem->Render();
    step_text->Set_Text("Steps: " + to_string(game_manager->Get_Current_Step()));
}

void Game_Scene::Update(std::chrono::milliseconds) {
    game_manager->Update();
}

void Game_Scene::On_Disconnected() {
    card_game.set_ui_screen(MENU);
    card_game.Close_Network();
}

void Game_Scene::On_Server_Stop() {
    card_game.set_ui_screen(MENU);
    card_game.Close_Network();
}
