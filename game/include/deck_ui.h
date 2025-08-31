#pragma once
#include "deck.h"
#include "game_object_ui.h"

class Deck_UI : public Game_Object_UI<Deck> {
  public:
    Deck_UI(Deck* deck, Game_UI_Manager& game_ui_manager) : Game_Object_UI(deck, game_ui_manager) {}

    void Update_UI() override {
        // game_ui_manager.DrawImage(object->.texture, object->pos, object->rot,
        // object->scale, object->color);
    }
};