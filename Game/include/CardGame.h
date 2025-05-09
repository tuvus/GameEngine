#pragma once

#include "Application.h"

#include <chrono>
#include <raylib.h>

using namespace std;

#define GRID_W 16
#define GRID_H 9
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TITLE_FONT_SIZE 24

enum SCREEN { MENU, GAME };

typedef struct Tile {
    Color color;
} Tile;

class Card_Game : public Application {
  public:
    Card_Game(bool client) : Application("CARD GAME", client, SCREEN_WIDTH, SCREEN_HEIGHT) {};

    void Init_Client() override;
    void Update(chrono::milliseconds s, Application&) override;
    void Render(chrono::milliseconds s, Application&) override;

    ~Card_Game();

    EUI_Context curr_ctx = EUI_Context();
    std::unordered_map<SCREEN, EUI_Element*> ui_screens;
    SCREEN screen = MENU;
};
