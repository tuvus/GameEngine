#include "CardGame.h"
#include "Application.h"
#include "build/vcpkg_installed/arm64-osx/include/raylib.h"
#include <raylib.h>

void resize_update(Card_Game& g) {
    if (g.curr_ctx.root) {
        g.curr_ctx.root->dim = {(float) g.screen_width, (float) g.screen_height};
    }

    g.curr_ctx.Perform_Layout();
}

void set_ui_screen(Card_Game& g, SCREEN screen) {
    g.curr_ctx.root = g.ui_screens[screen];
    g.curr_ctx.Perform_Layout();
}

void draw_UI(Card_Game& g) {
    g.curr_ctx.Begin_Frame();
    g.curr_ctx.Update_Input();
    g.curr_ctx.Handle_Input();
    g.curr_ctx.Render();
    g.curr_ctx.End_Frame();
}

EUI_Element* init_menu_ui(Card_Game& g) {
    auto* root = new EUI_HBox();
    root->pos = {0, 0};
    root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    root->style.background_color = RAYWHITE;
    root->style.font_size = 50;
    root->style.vertical_alignment = Alignment::Center;
    root->style.horizontal_alignment = Alignment::Center;

    root->Add_Child(new EUI_Button("Play", [&g]() {
        /*g.screen = GAME;*/
        set_ui_screen(g, GAME);
    }));

    return root;
}

EUI_Element* init_game_ui(Card_Game& g) {
    auto* root = new EUI_HBox();
    root->pos = {0, 0};
    root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    root->style.font_size = 50;
    root->style.vertical_alignment = Alignment::Center;
    root->style.horizontal_alignment = Alignment::Center;

    auto* button = new EUI_Button("Menu", [&g]() {
        /*g.screen = MENU;*/
        set_ui_screen(g, MENU);
    });
    root->Add_Child(button);

    return root;
}

void Card_Game::Init_Client() {
    SetWindowFocused();

    resize_update(*this);

    ui_screens.insert({MENU, init_menu_ui(*this)});
    ui_screens.insert({GAME, init_game_ui(*this)});

    screen = MENU;
    set_ui_screen(*this, MENU);
}

void Card_Game::Update(chrono::milliseconds deltaTime, Application& a) {
    if (IsKeyPressed(KEY_F)) {
        ToggleFullscreen();
        screen_width = GetScreenWidth();
        screen_height = GetScreenHeight();

        resize_update(*this);
    }

    if (IsKeyPressed(KEY_B)) {
        ToggleBorderlessWindowed();
        screen_width = GetScreenWidth();
        screen_height = GetScreenHeight();

        resize_update(*this);
    }

    if (WindowShouldClose())
        Close_Application();
}

void draw_menu(Card_Game& g) {
    BeginDrawing();

    draw_UI(g);

    EndDrawing();
}

void draw_game(Card_Game& g) {
    BeginDrawing();

    draw_UI(g);

    EndDrawing();
}

void Card_Game::Render(chrono::milliseconds deltaTime, Application& a) {
    if (curr_ctx.root) {
        ClearBackground(curr_ctx.default_style.background_color.value());
    } else {
        ClearBackground(BLACK);
    }

    switch (screen) {
        case MENU:
            draw_menu(*this);
            break;
        case GAME:
            draw_game(*this);
            break;
    }
}

Card_Game::~Card_Game() = default;
