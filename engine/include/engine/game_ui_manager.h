#pragma once
#include "game_manager.h"

#include <chrono>

class Object_UI;
class Game_UI_Manager {
  private:
    Application& application;
    Game_Manager& game_manager;
    std::unordered_map<Game_Object*, Object_UI*> active_ui_objects;
    std::unordered_set<Game_Object*> to_create;
    std::unordered_set<Game_Object*> to_delete;

    void On_Create_Object(Game_Object*);
    void On_Delete_Object(Game_Object*);
    static Game_UI_Manager* game_ui_manager_instance;

  public:
    Camera2D camera;

    Game_UI_Manager(Application& application, Game_Manager& game_manager);

    void Update_UI(std::chrono::milliseconds delta_time);

    void DrawImage(Texture2D& texture, Vector2 pos, float rot, float scale, Color color);

    void DrawScreenImage(Texture2D& texture, Vector2 pos, float rot, float scale, Color color);
};