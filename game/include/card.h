#pragma once
#include "card_player.h"
#include "game_object.h"

#include <functional>
#include <raymath.h>
#include <string>

class Card : public Game_Object {
  public:
    std::string name;
    std::string desc;
    int cost;
    std::function<void(Card_Player*)> effect;

    Card(Game_Manager& game_manager, Texture texture, std::string name, std::string desc, int cost,
         std::function<void(Card_Player*)> effect)
        : Game_Object(game_manager, texture, Vector2Zero(), 0, 1, WHITE), name(name), desc(desc),
          cost(cost), effect(effect) {}

    void Play_Card(Card_Player* card_player) {
        card_player->money -= cost;
        effect(card_player);
    }
};