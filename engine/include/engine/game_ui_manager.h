#pragma once
#include "game_manager.h"

#include <chrono>

class Game_UI_Manager {
  private:
    Game_Manager& game_manager;

  public:
    Game_UI_Manager(Game_Manager& game_manager) : game_manager(game_manager) {}

    void Update_UI(std::chrono::milliseconds delta_time) {
        for (const auto* obj : game_manager.Get_All_Objects()) {
            DrawTexturePro(
                obj->texture, {0, 0, (float) obj->texture.width, (float) obj->texture.height},
                {obj->pos.x, obj->pos.y, (float) obj->texture.width, (float) obj->texture.height},
                Vector2(obj->texture.width / 2, obj->texture.height / 2), obj->rot, WHITE);
        }
    }
};