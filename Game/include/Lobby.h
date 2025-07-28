#pragma once
#include "CardGame.h"
#include "ui/eui.h"

using namespace std;

class Lobby : public EUI_VBox {
public:
    Lobby(Card_Game& g);
};