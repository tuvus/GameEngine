#pragma once
#include "CardGame.h"
#include "Scene.h"

using namespace std;

class Lobby_Scene : public Scene, Network_Events_Receiver {
private:
    EUI_Text* status_text;
    EUI_Button* start_button;
    Card_Game& card_game;
    int player_count;
  public:
    Lobby_Scene(Card_Game& card_game);
    void Update_UI(std::chrono::milliseconds, EUI_Context context) override;
    void Update(std::chrono::milliseconds) override;
    void On_Connected() override;
    void On_Disconnected() override;
    void On_Server_Start() override;
    void On_Server_Stop() override;
    void On_Client_Connected(int) override;
    void On_Client_Disconnected(int) override;

};
