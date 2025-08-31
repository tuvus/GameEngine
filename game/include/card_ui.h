#pragma once
#include "card.h"
#include "game_object_ui.h"

class Card_UI : public Game_Object_UI<Card> {
  public:
    Card_UI(Card* card, Game_UI_Manager& game_ui_manager) : Game_Object_UI(card, game_ui_manager) {}

    void Update_UI() override {}
};