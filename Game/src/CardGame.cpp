#include "CardGame.h"
#include <iostream>
#include <raylib.h>

void Card_Game::update_function(chrono::milliseconds deltaTime, Application& a)
{
    if (IsKeyDown(KEY_A))
    {
        player.position.x -=
            MOVEMENT_SPEED *
            std::chrono::duration<float>(std::chrono::duration<float>(deltaTime).count()).count();
    }
    else if (IsKeyDown(KEY_D))
    {
        player.position.x += MOVEMENT_SPEED * std::chrono::duration<float>(deltaTime).count();
    }

    if ((IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_W)) && player.canJump)
    {
        player.velocity.y -= JUMP_SPEED;
        player.position.y += player.velocity.y * std::chrono::duration<float>(deltaTime).count();
        player.canJump = false;
    }

    if (player.position.y <= SCREEN_HEIGHT - 130 - PLAYER_HEIGHT)
    {
        player.position.y += player.velocity.y * std::chrono::duration<float>(deltaTime).count();
        player.velocity.y += GRAVITY * std::chrono::duration<float>(deltaTime).count();
        player.canJump = false;
    }
    else
    {
        player.velocity.y = 0.0f;
        player.position.y = SCREEN_HEIGHT - 130 - PLAYER_HEIGHT;
        player.canJump = true;
    }

    playerRect.x = player.position.x;
    playerRect.y = player.position.y;

    camera.target =
        (Vector2) {player.position.x + player.size.x / 2, player.position.y + player.size.y / 2};

    // Camera zoom controls
    camera.zoom += ((float) GetMouseWheelMove() * 0.05f);

    if (camera.zoom > 3.0f)
        camera.zoom = 3.0f;
    else if (camera.zoom < 0.1f)
        camera.zoom = 0.1f;

    // Camera reset (zoom)
    if (IsKeyPressed(KEY_R))
    {
        camera.zoom = 1.0f;
    }

    if (WindowShouldClose())
        Close_Application();
}

void Card_Game::render_function(chrono::milliseconds deltaTime, Application& a)
{
    // Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);
    DrawRectangle(-6000.0f, 450.0f - 130.0f, 13000.0f, 4500.0f, GRAY);
    for (int i = 0; i < MAX_BUILDINGS; i++)
    {
        if (buildings[i].x < camera.target.x - SCREEN_WIDTH ||
            buildings[i].x > camera.target.x + SCREEN_WIDTH)
        {
            continue;
        }
        DrawRectangleRec(buildings[i], buildingColors[i]);
    }
    DrawRectangleRec(playerRect, RED);
    EndMode2D();

    DrawFPS(10, 10);
    EndDrawing();
}

Card_Game::~Card_Game() = default;
