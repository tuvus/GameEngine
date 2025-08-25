#pragma once
#include "game_manager.h"
#include "networking/network.h"

class Player {
  public:
    Client_ID client_id;
    Player_ID player_id;
    bool local_player;

    Player(Client_ID client_id, Player_ID player_id) : client_id(client_id), player_id(player_id) {}
};