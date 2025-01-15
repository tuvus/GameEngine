#include "Application.h"
#include "ApplicationFactory.h"
#include "ApplicationFactoryTestUtils.h"
#include "gtest/gtest.h"

TEST(GTest, ServerSetsUp) {
    int* frames = new int();
    ApplicationFactory* test_factory = CreateApplicationTestFactory(
        [frames](chrono::milliseconds delta_time, ApplicationWindow& window) {
            if (*frames == 0) window.Get_Application().Start_Server();
            if (*frames == 5) window.Get_Application().Close_Application();
            ++*frames;
        });
    Create_Application(*test_factory, true);
    delete frames;
    delete test_factory;
}
