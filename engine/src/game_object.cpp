#include "game_object.h"
#include "game_manager.h"

Game_Object::Game_Object(Game_Manager& game_manager, Texture texture, const Vector2 pos,
                         const float rot)
    : texture(texture), id(game_manager.Get_New_Id()), pos(pos), rot(rot),
      game_manager(game_manager) {
}

Game_Object::~Game_Object() {
}

long Game_Object::Get_Id() const {
    return id;
}
