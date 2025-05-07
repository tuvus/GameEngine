#include "CardGame.h"
#include "build/vcpkg_installed/arm64-osx/include/raylib.h"
#include <raylib.h>

void resize_update(Card_Game& g) {
    g.tile_size = g.screen_height / GRID_H;
    g.tl_x = (g.screen_width - (g.tile_size * GRID_W)) / 2;
    g.tl_y = (g.screen_height - (g.tile_size * GRID_H)) / 2;

    g.center_x = (g.screen_width - MeasureText(g.Get_Name().c_str(), TITLE_FONT_SIZE)) / 2;
    if (g.ctx.root) {
        g.ctx.root->dim = {(float) g.screen_width, (float) g.screen_height};
    }

    g.ctx.Perform_Layout();
}

void Card_Game::Init_Client() {
    SetWindowFocused();

    resize_update(*this);

    auto* root = new EUI_HBox();
    /*root->style.vertical_alignment = Alignment::Center;*/
    /*root->style.horizontal_alignment = Alignment::Center;*/
    root->style.background_color = RAYWHITE;

    root->pos = {0, 0};
    root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    root->padding = {250, 250};

    auto* v = new EUI_VBox();
    auto* h = new EUI_HBox();
    auto* l = new EUI_VBox();
    l->style.background_color = LIME;
    auto* r = new EUI_HBox();
    r->style.background_color = BROWN;
    h->Add_Child(l);
    h->Add_Child(r);

    /*v->pos = {0, 0};*/
    /*v->dim = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT};*/

    v->style.background_color = LIGHTGRAY;
    v->style.border_thickness = 1;
    v->style.border_color = RED;

    v->style.horizontal_alignment = Alignment::Center;
    v->style.vertical_alignment = Alignment::Center;
    /*v->style.horizontal_alignment = Alignment::Start;*/
    /*v->style.vertical_alignment = Alignment::Start;*/
    /*v->style.horizontal_alignment = Alignment::Center;*/
    /*v->style.vertical_alignment = Alignment::End;*/

    /*v->padding = {15, 15};*/

    /*h->pos = {SCREEN_WIDTH / 2.0f, 0};*/
    /*h->dim = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT};*/

    h->style.background_color = GRAY;
    h->style.border_thickness = 1;
    h->style.border_color = BLUE;

    h->style.horizontal_alignment = Alignment::Center;
    h->style.vertical_alignment = Alignment::Center;
    /*h->style.horizontal_alignment = Alignment::Start;*/
    /*h->style.vertical_alignment = Alignment::Start;*/
    /*h->style.horizontal_alignment = Alignment::End;*/
    /*h->style.vertical_alignment = Alignment::Center;*/

    h->padding = {15, 15};

    root->Add_Child(v);
    root->Add_Child(h);

    auto* label = new EUI_Text("Card Game!");
    label->style.font_size = 50;
    v->Add_Child(label);
    /*root->Add_Child(label);*/

    /*auto* label2 = new EUI_Label("YUHHH!");*/
    /*label2->style.font_size = 40;*/
    /*v->Add_Child(label2);*/
    /*root->Add_Child(label2);*/

    /*auto* label3 = new EUI_Label("WOWOW!");*/
    /*label3->style.font_size = 40;*/
    /*h->Add_Child(label3);*/
    /*root->Add_Child(label3);*/

    auto* button = new EUI_Button("Play", [this] { screen = GAME; });
    button->style.font_size = 30;
    button->padding = {10, 5};
    /*button->dim.x = 200;*/
    /*button->dim.y = 100;*/
    l->Add_Child(button);
    /*root->Add_Child(button);*/

    ctx.root = root;
    ctx.Perform_Layout();
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

    g.ctx.Begin_Frame();
    g.ctx.Update_Input();
    /*g.ctx.Perform_Layout();*/
    g.ctx.Handle_Input();
    g.ctx.Render();
    g.ctx.End_Frame();

    EndDrawing();
}

void draw_game(Card_Game& g) {
    // Draw
    BeginDrawing();
    ClearBackground(BLACK);

    for (int y = g.tl_y; y < g.tl_y + GRID_H * g.tile_size; y += g.tile_size) {
        for (int x = g.tl_x; x < g.tl_x + GRID_W * g.tile_size; x += g.tile_size) {
            unsigned char red = y % 255;
            unsigned char blue = x % 255;
            unsigned char green = (x + y) % 255;
            DrawRectangleLines(x, y, g.tile_size, g.tile_size, Color{red, green, blue, 255});
        }
    }

    EndDrawing();
}

void Card_Game::Render(chrono::milliseconds deltaTime, Application& a) {
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
