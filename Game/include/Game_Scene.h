#pragma once
#include "CardGame.h"
#include "Scene.h"
#include "ui/eui.h"

class Game_Scene : public Scene, Network_Events_Receiver {
private:
    Card_Game& card_game;
public:
    Game_Scene(Card_Game& card_game) : Scene(card_game), card_game(card_game) {
        EUI_HBox* root = new EUI_HBox();
        root_elem = root;
        root->pos = {0, 0};
        root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
        root->style.vertical_alignment = Alignment::Center;
        root->style.horizontal_alignment = Alignment::Center;

        auto* button = new EUI_Button("Menu", [&card_game] {
            card_game.Close_Network();
            card_game.set_ui_screen(MENU);
        });
        button->style.padding = {10, 20, 10, 20};
        root->Add_Child(button);
        card_game.Get_Network()->connection_events->emplace(static_cast<Network_Events_Receiver*>(this));
    }
    ~Game_Scene() override {
        if (card_game.Get_Network() != nullptr)
            card_game.Get_Network()->connection_events->erase(static_cast<Network_Events_Receiver*>(this));
    }

    void Update_UI(std::chrono::milliseconds, EUI_Context context) override {
        root_elem->Render(context);
    }
    void Update(std::chrono::milliseconds) override {}

    void On_Connected() override {
    }

    void On_Disconnected() override {
        card_game.set_ui_screen(MENU);
        card_game.Close_Network();
    }

    void On_Server_Start() override {
    }

    void On_Server_Stop() override {
        card_game.set_ui_screen(MENU);
        card_game.Close_Network();
    }

    void On_Client_Connected(int) override {
    }

    void On_Client_Disconnected(int) override {
    }
};
