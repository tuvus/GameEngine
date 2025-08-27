#pragma once
#include "game_object.h"
#include "path.h"

class Unit : public Game_Object {
  public:
    Path* path;
    int section;
    // Linear interpolation between the previous point and the current point
    // 0 means the start position and 1 means the end position of the segment on the path
    float lerp;
    float speed;
    int team;
    bool spawned;

    Unit(Game_Manager& game_manager, Texture texture, Path* path, float speed, int team,
         float scale)
        : Game_Object(game_manager, texture, path->positions[0], path->Get_Rotation_On_Path(0),
                      scale),
          path(path), section(0), lerp(0), speed(speed), team(team) {
        spawned = true;
    }

    void Update() override {
        if (!spawned)
            return;
        if (section + 1 == path->positions.size()) {
            pos = path->positions[section];
            Delete_Object();
            return;
        }

        float dist = Vector2Distance(path->positions[section], path->positions[section + 1]);
        lerp += speed / dist;
        if (lerp > 1) {
            lerp--;
            section++;
        }
        if (section + 1 == path->positions.size()) {
            pos = path->positions[section];
            Delete_Object();
            return;
        }

        for (auto* object : game_manager.Get_All_Objects()) {
            if (Unit* other = dynamic_cast<Unit*>(object)) {
                if (other->team == team || !other->spawned)
                    continue;

                if (Vector2Distance(pos, other->pos) > 60)
                    continue;

                // Collide
                spawned = false;
                other->spawned = false;
                game_manager.Delete_Object(this);
                game_manager.Delete_Object(other);
                return;
            }
        }

        pos = Vector2Lerp(path->positions[section], path->positions[section + 1], lerp);
        rot = path->Get_Rotation_On_Path(section);
    }
};