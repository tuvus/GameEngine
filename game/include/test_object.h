#pragma once
#include "game_object.h"

class Test_Object : public Game_Object {
  public:
    Test_Object(Game_Manager& game_manager, Vector2 pos, float rot)
        : Game_Object(game_manager, pos, rot) {}
    void Update() override { this->pos.y += 1; }
};