#pragma once
#include "game_manager.h"

#include <chrono>

class Game_UI_Manager {
  private:
    Application& application;
    Game_Manager& game_manager;

  public:
    Camera2D camera;

    Game_UI_Manager(Application& application, Game_Manager& game_manager)
        : application(application), game_manager(game_manager) {
        camera = {0};
        camera.target = {static_cast<float>(application.screen_width) / 2,
                         static_cast<float>(application.screen_height) / 2};
        camera.offset = {static_cast<float>(application.screen_width) / 2,
                         static_cast<float>(application.screen_height) / 2};
        camera.rotation = 0;
        camera.zoom = 1.0f;
    }

    void Update_UI(std::chrono::milliseconds delta_time) {
        BeginMode2D(camera);
        for (const auto* obj : game_manager.Get_All_Objects()) {
            DrawTexturePro(
                obj->texture, {0, 0, (float) obj->texture.width, (float) obj->texture.height},
                {obj->pos.x, obj->pos.y, (float) obj->texture.width, (float) obj->texture.height},
                Vector2(obj->texture.width / 2, obj->texture.height / 2), obj->rot, WHITE);
        }
        EndMode2D();
    }
};