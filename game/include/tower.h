#pragma once
#include "game_object.h"

class Tower : public Game_Object {
  public:
    int team;
    bool spawned;

    Tower(Game_Manager& game_manager, Texture2D texture, Vector2 pos, int team)
        : Game_Object(game_manager, texture, pos, 0), team(team) {
        spawned = true;
    }

    void Update() override {}
};