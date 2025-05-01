#include "CardGame.h"
#include <raylib.h>

void Card_Game::init_client()
{
    SetWindowFocused();
    HideCursor();
}

void Card_Game::update(chrono::milliseconds deltaTime, Application& a)
{
    if (IsKeyPressed(KEY_F))
        ToggleFullscreen();

    if (IsKeyPressed(KEY_B))
        ToggleBorderlessWindowed();

    if (WindowShouldClose())
        Close_Application();
}

void Card_Game::render(chrono::milliseconds deltaTime, Application& a)
{
    // Draw
    BeginDrawing();
    ClearBackground(BLACK);

    DrawRectangle(0, 0, 100, 100, RED);

    EndDrawing();
}

Card_Game::~Card_Game() = default;
