#include "Application.h"
#include "CardGame.h"
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

void Card_Game::Init_Client() {
    SetWindowFocused();

    resize_update(*this);

    ui_screens.insert({TEST, init_test1_ui()});

    screen = TEST;
    set_ui_screen(*this, TEST);
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

void Card_Game::Render(chrono::milliseconds deltaTime, Application& a) {
    ClearBackground(RAYWHITE);

    this->curr_ctx.Render();

    EndDrawing();
}

Card_Game::~Card_Game() = default;
