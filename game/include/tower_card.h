#pragma once
#include "card.h"
#include "tower.h"

class Tower_Card : public Card {
  public:
    Tower_Data& tower_data;

    Tower_Card(Game_Manager& game_manager, Game_Scene& game_scene, Card_Data& card_data,
               Tower_Data& tower_data)
        : Card(game_manager, game_scene, card_data), tower_data(tower_data) {}

    Card* Clone() { return new Tower_Card(game_manager, game_scene, card_data, tower_data); }

    void Play_Card(Card_Player* player) { Card::Play_Card(player); }

    void Update() override {}
};