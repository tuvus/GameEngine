#include "Application.h"
#include "ApplicationFactory.h"
#include "ApplicationFactoryTestUtils.h"
#include "gtest/gtest.h"

TEST(GTest, GameSetsUp) {
    ApplicationFactory* test_factory = CreateApplicationTestFactory(
        [](chrono::milliseconds delta_time, ApplicationWindow& window) {
            window.Get_Application().Close_Application();
        });
    Create_Application(*test_factory, true);
    delete test_factory;
}
