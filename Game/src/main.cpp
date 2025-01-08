#include "CardGame.h"
#include "Game.h"

int main() {
    auto* card_game = new Card_Game();
    Create_Game(*card_game);
    delete card_game;
}
