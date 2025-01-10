#include "Application.h"
#include "gtest/gtest.h"

class TestGame : public Application {
public:
    string Get_Name() override { return "TestGame"; }
    void Update_Application(std::chrono::milliseconds deltaTime) override {
        glfwSetWindowShouldClose(&GetWindow(), true);
    }
    void End_Application() override {}
};

TEST(GTest, GameSetsUp) {
    auto* testGame = new TestGame();
    Create_Application(*testGame, false);
    delete testGame;
}
