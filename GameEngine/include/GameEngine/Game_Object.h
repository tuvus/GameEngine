#pragma once

#include <raylib.h>

class Game_Manager;

class Game_Object {
private:
    const long id;
    Vector2 pos;
    float rot;
    Game_Manager& game_manager;

public:
    Game_Object(Game_Manager& game_manager, Vector2 pos, float rot);
    virtual ~Game_Object();
    virtual void Update() = 0;
    long Get_Id() const;

};