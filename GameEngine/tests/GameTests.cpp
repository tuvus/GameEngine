#include "Game.h"
#include "gtest/gtest.h"

class TestGame : public Game {
public:
    string Get_Name() override { return "TestGame"; }
    void Update_Game(std::chrono::milliseconds deltaTime) override {
        glfwSetWindowShouldClose(&GetWindow(), true);
    }
    void End_Game() override {}
};

TEST(GTest, GameSetsUp) {
    auto* testGame = new TestGame();
    Create_Game(*testGame, false);
    delete testGame;
}
