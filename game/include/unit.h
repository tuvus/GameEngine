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

    Unit(Game_Manager& game_manager, Texture texture, Path* path, float speed)
        : Game_Object(game_manager, texture, path->positions[0], path->Get_Rotation_On_Path(0)),
          path(path), section(0), lerp(0), speed(speed) {}

    void Update() override {
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
        pos = Vector2Lerp(path->positions[section], path->positions[section + 1], lerp);
        rot = path->Get_Rotation_On_Path(section);
    }
};