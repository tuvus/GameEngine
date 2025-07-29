#include "Lobby_Scene.h"

Lobby_Scene::Lobby_Scene(Card_Game& card_game) : Scene(card_game) {
    auto* root = new EUI_VBox();
    root_elem = root;
    root->pos = {0, 0};
    root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    root->style.vertical_alignment = Alignment::Center;
    root->style.horizontal_alignment = Alignment::Center;
    root->gap = 20;
    auto* players_text = new EUI_Text("Players: 0");
    root->Add_Child(players_text);
    auto* leave_button = new EUI_Button("Leave Room", [this, &card_game]() {
        card_game.Close_Network();
        card_game.set_ui_screen(MENU);
    });
    root->Add_Child(leave_button);
}

void Lobby_Scene::Update_UI(std::chrono::milliseconds, EUI_Context context) {
}

void Lobby_Scene::Update(std::chrono::milliseconds) {
}
