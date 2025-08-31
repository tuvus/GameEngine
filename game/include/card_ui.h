#pragma once
#include "card.h"
#include "game_object_ui.h"
#include "game_ui_manager.h"

class Card_UI : public Game_Object_UI<Card> {
  public:
    Card_UI(Card* card, Game_UI_Manager& game_ui_manager) : Game_Object_UI(card, game_ui_manager) {}

    void Update_UI() override {}

    void Draw_Card(Vector2 pos, float scale) {
        game_ui_manager.DrawImage(object->card_data.texture, pos, object->rot, scale,
                                  object->color);
        DrawText(object->card_data.name.c_str(),
                 pos.x - object->card_data.texture.width * scale / 2 + 5,
                 pos.y - object->card_data.texture.height * scale / 2 + 10, 20, BLACK);
        DrawText(to_string(object->card_data.cost).c_str(),
                 pos.x + object->card_data.texture.width * scale / 2 - 25,
                 pos.y - object->card_data.texture.height * scale / 2 + 45, 20, BLACK);
        DrawText(object->card_data.desc.c_str(),
                 pos.x - object->card_data.texture.width * scale / 2 + 5, pos.y, 10, BLACK);
    }
};