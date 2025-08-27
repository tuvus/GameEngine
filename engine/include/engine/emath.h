#pragma once
#include <cmath>
#include <raylib.h>
// A file for all of the Euclid math code

inline float Get_Rotation_From_Positions(Vector2 from, Vector2 to) {
    return std::atan2f(to.y - from.y, to.x - from.x) * RAD2DEG + 90;
}