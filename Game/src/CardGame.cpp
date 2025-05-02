#include "CardGame.h"
#include <raylib.h>

void resize_update(Card_Game& g)
{
    g.tile_size = g.screen_height / GRID_H;
    g.tl_x = (g.screen_width - (g.tile_size * GRID_W)) / 2;
    g.tl_y = (g.screen_height - (g.tile_size * GRID_H)) / 2;

    g.center_x = (g.screen_width - MeasureText(g.Get_Name().c_str(), TITLE_FONT_SIZE)) / 2;
}

void Card_Game::Init_Client()
{
    SetWindowFocused();
    HideCursor();

    resize_update(*this);
}

void Card_Game::Update(chrono::milliseconds deltaTime, Application& a)
{
    if (IsKeyPressed(KEY_F))
    {
        ToggleFullscreen();
        screen_width = GetScreenWidth();
        screen_height = GetScreenHeight();

        resize_update(*this);
    }

    if (IsKeyPressed(KEY_B))
    {
        ToggleBorderlessWindowed();
        screen_width = GetScreenWidth();
        screen_height = GetScreenHeight();

        resize_update(*this);
    }

    if (WindowShouldClose())
        Close_Application();
}

void draw_menu(Card_Game& g)
{
    BeginDrawing();

    DrawText(g.Get_Name().c_str(), g.center_x, 100, TITLE_FONT_SIZE, RAYWHITE);

    EndDrawing();
}

void draw_game(Card_Game& g)
{
    // Draw
    BeginDrawing();
    ClearBackground(BLACK);

    for (int y = g.tl_y; y < g.tl_y + GRID_H * g.tile_size; y += g.tile_size)
    {
        for (int x = g.tl_x; x < g.tl_x + GRID_W * g.tile_size; x += g.tile_size)
        {
            unsigned char red = y % 255;
            unsigned char blue = x % 255;
            unsigned char green = (x + y) % 255;
            DrawRectangleLines(x, y, g.tile_size, g.tile_size, Color{red, green, blue, 255});
        }
    }

    EndDrawing();
}

void Card_Game::Render(chrono::milliseconds deltaTime, Application& a)
{
    switch (screen)
    {
        case MENU:
            draw_menu(*this);
            break;
        case GAME:
            draw_game(*this);
            break;
    }
}

Card_Game::~Card_Game() = default;
