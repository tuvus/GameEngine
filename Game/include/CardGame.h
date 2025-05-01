#pragma once

#include "Application.h"

#include <chrono>
#include <raylib.h>

using namespace std;

#define GRID_H 64
#define GRID_W 64
#define SCREEN_HEIGHT 450
#define SCREEN_WIDTH 800

typedef struct Tile
{
    Color color;
} Tile;

class Card_Game : public Application
{
  public:
    Card_Game(bool client) : Application("Card Game", client, SCREEN_WIDTH, SCREEN_HEIGHT) {};

    void init_client() override;
    void update(chrono::milliseconds s, Application&) override;
    void render(chrono::milliseconds s, Application&) override;

    ~Card_Game();
};
