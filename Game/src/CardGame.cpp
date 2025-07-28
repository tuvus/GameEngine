#include "CardGame.h"
#include "Application.h"
#include "Lobby.h"
#include "networking/Network.h"
#include "rpc/client.h"

#include <iostream>
#include <raylib.h>

void resize_update(Card_Game& g) {
    if (g.curr_ctx.root) {
        g.curr_ctx.root->dim = {(float) g.screen_width, (float) g.screen_height};
    }

    g.curr_ctx.Perform_Layout();
}

void Card_Game::set_ui_screen(SCREEN screen) {
    curr_ctx.root = ui_screens[screen];
    curr_ctx.Perform_Layout();
}

void draw_UI(Card_Game& g) {
    g.curr_ctx.Render();
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

EUI_Element* Card_Game::init_menu_ui(Card_Game& g) {
    auto* root = new EUI_VBox();
    root->pos = {0, 0};
    root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    root->style.vertical_alignment = Alignment::Center;
    root->style.horizontal_alignment = Alignment::Center;
    root->gap = 20;

    auto* title = new EUI_Text("Game Title!");
    title->style.font_size = 40;
    root->Add_Child(title);


    auto* join_button = new EUI_Button("Join Game", [this, &g]() {
        Start_Client();
        g.screen = GAME;
        g.set_ui_screen(LOBBY);
    });
    join_button->style.padding = {10, 20, 10, 20};
    root->Add_Child(join_button);

    auto* host_button = new EUI_Button("Host Game", [this, &g]() {
        Start_Server();
        g.screen = GAME;
        g.set_ui_screen(LOBBY);
    });
    host_button->style.padding = {10, 20, 10, 20};
    root->Add_Child(host_button);

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
        g.set_ui_screen(MENU);
    });
    button->style.padding = {10, 20, 10, 20};
    root->Add_Child(button);

    return root;
}

void Card_Game::Init_Client() {
    SetWindowFocused();

    resize_update(*this);

    ui_screens.insert({MENU, init_menu_ui(*this)});
    ui_screens.insert({LOBBY, new Lobby(*this)});
    ui_screens.insert({GAME, init_game_ui(*this)});

    screen = MENU;
    set_ui_screen(MENU);
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
