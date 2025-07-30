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
    g.curr_ctx.Render();
}

EUI_Element* init_menu_ui(Card_Game& g) {
    auto* root = new EUI_VBox();
    root->pos = {0, 0};
    root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    root->style.vertical_alignment = Alignment::Center;
    root->style.horizontal_alignment = Alignment::Center;
    root->gap = 20;

    auto* title = new EUI_Text("Game Title!");
    title->style.font_size = 40;
    root->Add_Child(title);

    auto* button = new EUI_Button("Play", [&g]() {
        g.screen = GAME;
        set_ui_screen(g, GAME);
    });
    button->style.padding = {10, 20, 10, 20};
    button->style.font_size = 20;
    root->Add_Child(button);

    return root;
}

EUI_Element* init_game_ui(Card_Game& g) {
    auto* root = new EUI_HBox();
    root->pos = {0, 0};
    root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    root->style.vertical_alignment = Alignment::Center;
    root->style.horizontal_alignment = Alignment::Center;

    auto* button = new EUI_Button("Menu", [&g]() {
        g.screen = MENU;
        set_ui_screen(g, MENU);
    });
    button->style.padding = {10, 20, 10, 20};
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

    curr_ctx.Begin_Frame();
    curr_ctx.Update_Input();
    curr_ctx.Handle_Input();
    curr_ctx.End_Frame();

    if (WindowShouldClose())
        Close_Application();
}

void draw_menu(Card_Game& g) {
    draw_UI(g);
}

void draw_game(Card_Game& g) {
    draw_UI(g);
}

void Card_Game::Render(chrono::milliseconds deltaTime, Application& a) {
    ClearBackground(RAYWHITE);

    switch (screen) {
        case MENU:
            draw_menu(*this);
            break;
        case GAME:
            draw_game(*this);
            break;
    }

    EndDrawing();
}

Card_Game::~Card_Game() = default;
