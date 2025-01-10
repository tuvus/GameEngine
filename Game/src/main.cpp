#include "CardGame.h"
#include "Application.h"

int main() {
    auto* card_game = new Card_Game();
    Create_Application(*card_game, false);
    delete card_game;
}
