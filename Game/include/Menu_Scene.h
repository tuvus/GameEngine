#pragma once
#include "CardGame.h"
#include "Scene.h"

class Menu_Scene : public Scene {
public:
    Menu_Scene(Card_Game& card_game) : Scene(card_game) {
        EUI_VBox* root = new EUI_VBox();
        root_elem = root;
        root->pos = {0, 0};
        root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
        root->style.vertical_alignment = Alignment::Center;
        root->style.horizontal_alignment = Alignment::Center;
        root->gap = 20;

        auto* title = new EUI_Text("Game Title!");
        title->style.font_size = 40;
        root->Add_Child(title);

        auto* join_button = new EUI_Button("Join Game", [&card_game] {
            card_game.Connect_To_Server();
            card_game.set_ui_screen(LOBBY);
        });
        join_button->style.padding = {10, 20, 10, 20};
        root->Add_Child(join_button);

        auto* host_button = new EUI_Button("Host Game", [&card_game] {
            card_game.Start_Server();
            card_game.set_ui_screen(LOBBY);
        });
        host_button->style.padding = {10, 20, 10, 20};
        root->Add_Child(host_button);
    }
    void Update_UI(chrono::milliseconds, EUI_Context context) override {}
    void Update(chrono::milliseconds) override {}
};