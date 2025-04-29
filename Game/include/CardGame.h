#pragma once

#include "Application.h"

#include <chrono>
#include <raylib.h>

using namespace std;

#define MAX_BUILDINGS 100
#define MOVEMENT_SPEED 100
#define JUMP_SPEED 350
#define SCREEN_HEIGHT 450
#define SCREEN_WIDTH 800
#define PLAYER_HEIGHT 40
#define PLAYER_WIDTH 40
#define GRAVITY 800

typedef struct
{
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    bool canJump;
} Player;

typedef struct
{
    Rectangle rect;
    bool blocking;
    Color color;
} Platform;

class Card_Game : public Application
{
  public:
    Card_Game(bool client) : Application("Card Game", client) {};

    void update_function(chrono::milliseconds s, Application&) override;
    void render_function(chrono::milliseconds s, Application&) override;

    ~Card_Game();

    // TESTING
    Player player = {{SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 130 - PLAYER_HEIGHT},
                     {0.0f, 0.0f},
                     {PLAYER_WIDTH, PLAYER_HEIGHT},
                     true};
    Rectangle playerRect = {player.position.x, player.position.y, player.size.x, player.size.y};
    Rectangle buildings[MAX_BUILDINGS] = {0};
    Color buildingColors[MAX_BUILDINGS] = {0};
    Camera2D camera = {0};

    void init_game()
    {
        int offset = 0;
        for (int i = 0; i < MAX_BUILDINGS; i++)
        {
            buildings[i].width = (float) GetRandomValue(50, 200);
            buildings[i].height = (float) GetRandomValue(100, 800);
            buildings[i].x = -6000.0f + offset;
            buildings[i].y = SCREEN_HEIGHT - 130.0f - buildings[i].height;

            offset += buildings[i].width;
            buildingColors[i] = (Color) {static_cast<unsigned char>(GetRandomValue(200, 240)),
                                         static_cast<unsigned char>(GetRandomValue(200, 240)),
                                         static_cast<unsigned char>(GetRandomValue(200, 250)), 255};
        }

        camera.target = (Vector2) {player.position.x + player.size.x / 2,
                                   player.position.y + player.size.y / 2};
        camera.offset = (Vector2) {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
    }
};
