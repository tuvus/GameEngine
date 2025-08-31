#pragma once
#include "card.h"
#include "card_ui.h"
#include "deck.h"
#include "game_object_ui.h"
#include "game_ui_manager.h"

class Deck_UI : public Game_Object_UI<Deck> {
  public:
    Deck_UI(Deck* deck, Game_UI_Manager& game_ui_manager) : Game_Object_UI(deck, game_ui_manager) {}

    void Update_UI() override {
        float x_pos = 10;
        for (auto card : object->hand) {
            Card_UI* card_ui = static_cast<Card_UI*>(game_ui_manager.active_ui_objects[card]);
            card_ui->Draw_Card(Vector2(x_pos + card->card_data.texture.width / 8,
                                       game_object->game_manager.application.screen_height -
                                           card->card_data.texture.height / 8 - 10),
                               card->scale / 4);
            x_pos += card->card_data.texture.width / 4 + 10;
        }
    }
};