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

    auto* root = new EUI_VBox();
    root->pos = {0, 0};
    root->dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    root->padding = {10, 20, 30, 40};
    root->style.background_color = RAYWHITE;

    auto* green_box = new EUI_VBox();
    green_box->style.background_color = GREEN;
    auto* orange_box = new EUI_VBox();
    orange_box->style.background_color = ORANGE;
    auto* text1 = new EUI_Text("yo");
    text1->padding = {0, 100, 0, 0};
    text1->style.border_thickness = 1;
    auto* text2 = new EUI_Text("gurt");
    text1->padding = {0, 50, 0, 0};
    text1->style.border_thickness = 1;

    root->Add_Child(green_box);
    root->Add_Child(text1);
    root->Add_Child(orange_box);
    root->Add_Child(text2);

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
