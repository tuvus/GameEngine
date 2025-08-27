#pragma once
#include "emath.h"
#include "game_object.h"
#include "unit.h"

class Tower : public Game_Object {
  public:
    int team;
    bool spawned;
    int reload;
    float range;

    Tower(Game_Manager& game_manager, Texture2D texture, Vector2 pos, float range, int team,
          float scale, Color color)
        : Game_Object(game_manager, texture, pos, team == 0 ? 0 : 180, scale, color), range(range),
          team(team) {
        spawned = true;
        reload = 100;
    }

    void Update() override {
        if (reload == 0) {
            Vector2 home = Vector2(team == 0 ? game_manager.application.screen_height : 0,
                                   game_manager.application.screen_width / 2);
            Unit* closest_unit = nullptr;
            float dist = INT_MAX;
            for (auto* object : game_manager.Get_All_Objects()) {
                if (Unit* other = dynamic_cast<Unit*>(object)) {
                    if (other->team == team || !other->spawned)
                        continue;
                    if (Vector2Distance(pos, other->pos) > range)
                        continue;
                    float new_dist = Vector2Distance(other->pos, home);
                    if (new_dist >= dist)
                        continue;

                    closest_unit = other;
                    dist = new_dist;
                }
            }

            if (closest_unit != nullptr) {
                // Fire
                closest_unit->spawned = false;
                game_manager.Delete_Object(closest_unit);
                reload = 100;
                rot = Get_Rotation_From_Positions(pos, closest_unit->pos);
            }
        } else {
            reload--;
        }
    }
};