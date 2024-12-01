
#include "CardGame.h"
#include "Game.h"

int main() {
    auto* cardGame = new CardGame();
    CreateGame(*cardGame);
    delete cardGame;
  }
