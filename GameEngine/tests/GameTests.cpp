#include "Application.h"
#include "ApplicationFactory.h"
#include "gtest/gtest.h"

class TestWindow : public ApplicationWindow {
public:
    TestWindow(Application& application): ApplicationWindow(application) {}
    void Render(std::chrono::milliseconds deltaTime) override {
        Get_Application().Close_Application();
    }
};

class TestFactory : public ApplicationFactory {
public:
    TestFactory() = default;
    string Get_Name() override { return "TestGame"; }

    ApplicationWindow* Create_Window(Application& application) override {
        return new TestWindow(application);
    }

    ~TestFactory() override = default;
};

TEST(GTest, GameSetsUp) {
    auto* test_factory = new TestFactory();
    Create_Application(*test_factory, false);
    delete test_factory;
}
