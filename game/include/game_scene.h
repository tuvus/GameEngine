#pragma once

#include "card_game.h"
#include "game_manager.h"
#include "path.h"
#include "scene.h"
#include "ui/eui.h"

class Game_Scene : public Scene, Network_Events_Receiver {
  private:
    Card_Game& card_game;
    std::unique_ptr<Game_Manager> game_manager;
    EUI_Text* step_text;
    Path* path;

  public:
    Game_Scene(Card_Game& card_game);

    ~Game_Scene() override;
    void Setup_Scene(unordered_map<Client_ID, Player_ID>* clients_players, Player_ID player_id);

    void Update_UI(chrono::milliseconds) override;
    void Update(std::chrono::milliseconds) override { game_manager->Update(); }

    void On_Connected() override {}

    void On_Disconnected() override;

    void On_Server_Start() override {}

    void On_Server_Stop() override;

    void On_Client_Connected(Client_ID) override {}

    void On_Client_Disconnected(Client_ID) override {}
};
