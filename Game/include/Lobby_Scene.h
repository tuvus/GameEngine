#pragma once
#include "CardGame.h"
#include "Scene.h"

using namespace std;

class Lobby_Scene : public Scene {
private:
    EUI_Text* status_text;
    Card_Game& card_game;
    int player_count;
  public:
    Lobby_Scene(Card_Game& card_game);
    void Update_UI(std::chrono::milliseconds, EUI_Context context) override;
    void Update(std::chrono::milliseconds) override;
};
