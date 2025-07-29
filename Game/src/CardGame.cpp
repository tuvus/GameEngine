#include "CardGame.h"
#include "Application.h"
#include "Game_Scene.h"
#include "Lobby_Scene.h"
#include "Menu_Scene.h"
#include "networking/Network.h"
#include "rpc/client.h"

#include <iostream>
#include <raylib.h>

void Card_Game::resize_update() {
    if (curr_ctx.root) {
        curr_ctx.root->dim = {(float) screen_width, (float) screen_height};
    }

    curr_ctx.Perform_Layout();
}

void Card_Game::set_ui_screen(SCREEN new_screen) {
    delete scene;
    scene = sceens[new_screen]();
    curr_ctx.root = scene->Get_Root();
    curr_ctx.Perform_Layout();
}

EUI_Element* init_test1_ui() {
    auto* root = new EUI_HBox();
    root->pos = {0, 0};
    root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    root->style.vertical_alignment = Alignment::Center;
    root->style.horizontal_alignment = Alignment::Center;
    root->style.padding = {10, 20, 10, 20};

    auto* orange = new EUI_VBox();
    orange->style.background_color = ORANGE;
    root->Add_Child(orange);
    orange->style.padding = {20, 20, 20, 20};

    auto* gold = new EUI_VBox();
    gold->style.background_color = GOLD;
    gold->style.vertical_alignment = Alignment::Center;
    gold->style.horizontal_alignment = Alignment::Center;
    root->Add_Child(gold);
    gold->style.padding = {20, 20, 20, 20};

    auto* purple = new EUI_VBox();
    purple->style.background_color = PURPLE;
    purple->style.vertical_alignment = Alignment::End;
    purple->style.horizontal_alignment = Alignment::End;
    root->Add_Child(purple);
    purple->style.padding = {20, 20, 20, 20};

    auto* text1 = new EUI_Text("this");
    text1->style.font_size = 40;
    text1->style.padding = {400};
    orange->Add_Child(text1);

    auto* lime = new EUI_VBox();
    lime->style.background_color = LIME;
    lime->style.vertical_alignment = Alignment::Center;
    lime->style.horizontal_alignment = Alignment::End;
    orange->Add_Child(lime);

    auto* wow = new EUI_Text("WOW!");
    lime->Add_Child(wow);

    auto* text2 = new EUI_Text("is");
    text2->style.font_size = 40;
    gold->Add_Child(text2);

    auto* text3 = new EUI_Text("auto-formatted!");
    text3->style.font_size = 40;
    purple->Add_Child(text3);

    return root;
}

void Card_Game::Start_Client() {
    Application::Start_Client();
    SetWindowFocused();

    resize_update();
    sceens.insert({MENU, [this]() -> Scene* { return new Menu_Scene(*this); }});
    sceens.insert({LOBBY, [this]() -> Scene* { return new Lobby_Scene(*this); }});
    sceens.insert({GAME, [this]() -> Scene* { return new Game_Scene(*this); }});

    set_ui_screen(MENU);
}

void Card_Game::Update(chrono::milliseconds deltaTime) {
}

void Card_Game::Update_UI(chrono::milliseconds deltaTime) {
    if (IsKeyPressed(KEY_F)) {
        ToggleFullscreen();
        screen_width = GetScreenWidth();
        screen_height = GetScreenHeight();

        resize_update();
    }

    if (IsKeyPressed(KEY_B)) {
        ToggleBorderlessWindowed();
        screen_width = GetScreenWidth();
        screen_height = GetScreenHeight();

        resize_update();
    }

    curr_ctx.Begin_Frame();
    curr_ctx.Update_Input();
    curr_ctx.Handle_Input();
    curr_ctx.End_Frame();
    ClearBackground(RAYWHITE);

    scene->Update_UI(deltaTime, curr_ctx);

    if (WindowShouldClose())
        Close_Application();

    EndDrawing();

    curr_ctx.Render();
}

Card_Game::~Card_Game() = default;
