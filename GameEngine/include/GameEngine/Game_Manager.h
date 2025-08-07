#pragma once
#include "Application.h"

class Game_Manager {
  private:
    Application& application;
    Network& network;

    long player_id;
    unordered_map<long, long> player_ids;
    /* The current count of the step */
    long step = 0;
    /* The maximum step that we are allowed to simulate to */
    long max_step = 0;
    /* The lowest step that a cilent is on.
     * Used by the server to determine if it should continue or wait. */
    long min_step = 0;
    /* Used on the server to store what steps */
    std::unordered_map<long, long> player_steps;
    void On_Recieve_Step_Update(long max_step);
    /* Called on the client to update the server on its current step */
    void On_Recieve_Player_Step_Update(long player_id, long min_step);
    long next_id;

  public:
    Game_Manager(Application&, Network&);
    ~Game_Manager();
    void Update();
    long Get_New_Id();
};