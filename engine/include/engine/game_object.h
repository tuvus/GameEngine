#pragma once

#include <raylib.h>

class Game_Manager;

class Game_Object {
  public:
    const long id;
    Vector2 pos;
    float rot;
    Game_Manager& game_manager;

    Game_Object(Game_Manager& game_manager, Vector2 pos, float rot);
    virtual ~Game_Object();
    virtual void Update() = 0;
    long Get_Id() const;
};