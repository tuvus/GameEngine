#include "CardGame.h"
#include "Application.h"
#include "Game_Scene.h"
#include "Lobby_Scene.h"
#include "Menu_Scene.h"

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

    if (IsKeyPressed(KEY_Q) && IsKeyDown(KEY_LEFT_CONTROL)) {
        Close_Application();
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
