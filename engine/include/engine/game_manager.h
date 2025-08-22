#pragma once
#include "application.h"

typedef long Player_ID;

class Game_Manager {
  private:
    Application& application;
    Network& network;

    Player_ID player_id;
    unordered_map<Client_ID, Player_ID> client_id_to_player_id;
    unordered_map<Player_ID, Client_ID> player_id_to_client_id;
    /* The current count of the step */
    long step = 0;
    /* The maximum step that we are allowed to simulate to */
    long max_step = 0;
    /* The lowest step that a cilent is on.
     * Used by the server to determine if it should continue or wait. */
    long min_step = 0;
    /* Used on the server to store what steps */
    std::unordered_map<Player_ID, long> player_steps;
    /* Called on clients from the server to set the max step */
    void On_Receive_Step_Update(long max_step);
    /* Called on the server from the client to inform the server on its current step */
    void On_Receive_Player_Step_Update(Player_ID player_id, long min_step);
    long next_id;

  public:
    Game_Manager(Application&, Network&, unordered_map<Client_ID, Player_ID>*, Player_ID);
    ~Game_Manager();
    void Update();
    long Get_New_Id();
    long Get_Current_Step() const;
    Client_ID Get_Client_ID(Player_ID);
};
