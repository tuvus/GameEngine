#pragma once

#include <raylib.h>

class Game_Manager;

class Game_Object {
  public:
    const long id;
    Vector2 pos;
    float rot;
    Game_Manager& game_manager;
    Texture2D texture;
    float scale;
    Color color;

    Game_Object(Game_Manager& game_manager, Texture2D texture, Vector2 pos, float rot, float scale,
                Color color);
    void Delete_Object();
    virtual ~Game_Object();
    virtual void Update() = 0;
    long Get_Id() const;
};