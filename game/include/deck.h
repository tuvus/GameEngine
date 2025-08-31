#pragma once
#include "game_object.h"

#include <raymath.h>

class Deck : public Game_Object {
  public:
    Deck(Game_Manager& game_manager, Texture texture)
        : Game_Object(game_manager, texture, Vector2Zero(), 0, 1, WHITE) {}
};