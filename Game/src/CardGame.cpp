#include <raylib.h>

#include "Application.h"
#include "CardGame.h"
#include "Game_Scene.h"
#include "Lobby_Scene.h"
#include "Menu_Scene.h"

void Card_Game::resize_update() {
    auto root = scene->Get_Root();
    if (root) {
        root->dim = {(float) screen_width, (float) screen_height};
    }

    // TODO: should we panic if ctx is null?
    if (eui_ctx)
        eui_ctx->Perform_Layout();
}

void Card_Game::set_ui_screen(SCREEN new_screen) {
    delete scene;
    scene = sceens[new_screen]();
    // will panic if eui_ctx is null, shouldn't ever happen so let it crash
    eui_ctx->Set_Root(scene->Get_Root());
    eui_ctx->Perform_Layout();
}

void Card_Game::Start_Client() {
    Application::Start_Client();

    sceens.insert({MENU, [this]() -> Scene* { return new Menu_Scene(*this); }});
    sceens.insert({LOBBY, [this]() -> Scene* { return new Lobby_Scene(*this); }});
    sceens.insert({GAME, [this]() -> Scene* { return new Game_Scene(*this); }});

    set_ui_screen(MENU);

    resize_update();
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
        return;
    }

    eui_ctx->Begin_Frame();
    eui_ctx->Update_Input();
    eui_ctx->Handle_Input();
    eui_ctx->End_Frame();

    if (WindowShouldClose()) {
        Close_Application();
        return;
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);
    scene->Update_UI(deltaTime);

    EndDrawing();
}

Card_Game::~Card_Game() {
    delete scene;
    scene = nullptr;
}

void Card_Game::Close_Application() {
    delete scene;
    scene = nullptr;
    Application::Close_Application();
}
