#pragma once
#include "CardGame.h"
#include "Scene.h"
#include "ui/eui.h"

class Game_Scene : public Scene {
public:
    Game_Scene(Card_Game& card_game) : Scene(card_game) {
        EUI_HBox* root = new EUI_HBox();
        root_elem = root;
        root->pos = {0, 0};
        root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
        root->style.vertical_alignment = Alignment::Center;
        root->style.horizontal_alignment = Alignment::Center;

        auto* button = new EUI_Button("Menu", [&card_game] { card_game.set_ui_screen(MENU); });
        button->style.padding = {10, 20, 10, 20};
        root->Add_Child(button);
    }

    void Update_UI(std::chrono::milliseconds, EUI_Context context) override {
        root_elem->Render(context);
    }
    void Update(std::chrono::milliseconds) override {}
};
