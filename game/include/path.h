#pragma once
#include "game_object.h"

#include "raymath.h"
#include <vector>

class Path {
  public:
    std::vector<Vector2> positions;

    Path(std::vector<Vector2> positions) : positions(positions) {}
    float Get_Rotation_On_Path(int progress) const {
        if (progress > positions.size())
            return 0;
        return atan2f(positions[progress + 1].y - positions[progress].y,
                      positions[progress + 1].x - positions[progress].x) *
                   RAD2DEG +
               90;
    }
};