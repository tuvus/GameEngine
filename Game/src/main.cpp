#include "CardGame.h"
#include "Game.h"

int main() {
    auto* cardGame = new Card_Game();
    Create_Game(*cardGame);
    delete cardGame;
}
