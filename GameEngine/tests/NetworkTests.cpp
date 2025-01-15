#include "Application.h"
#include "ApplicationFactory.h"
#include "gtest/gtest.h"

class NetworkTestWindow : public ApplicationWindow {
public:
    NetworkTestWindow(Application& application): ApplicationWindow(application) {}
    int frames = 0;

    void Render(std::chrono::milliseconds deltaTime) override {
        if (frames == 0) Get_Application().Start_Server();
        if (frames == 5) Get_Application().Close_Application();
        ++frames;
    }
};

class NetworkTestFactory : public ApplicationFactory {
public:
    NetworkTestFactory() = default;
    string Get_Name() override { return "TestGame"; }

    ApplicationWindow* Create_Window(Application& application) override {
        return new NetworkTestWindow(application);
    }

    ~NetworkTestFactory() override = default;
};

TEST(GTest, ServerSetsUp) {
    auto* test_factory = new NetworkTestFactory();
    Create_Application(*test_factory, true);
    delete test_factory;
}
