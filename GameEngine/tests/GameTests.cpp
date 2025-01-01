#include "Game.h"
#include "gtest/gtest.h"

class TestGame : public Game {
public:
    string GetName() override { return "TestGame"; }
    void UpdateGame(std::chrono::milliseconds deltaTime) override {}
    void EndGame() override {}
};

TEST(GTest, GameSetsUp) {
    auto* testGame = new TestGame();
    CreateGame(*testGame);
    delete testGame;
}
