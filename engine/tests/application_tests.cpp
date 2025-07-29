#include "application.h"
#include "application_factory.h"
#include "application_factory_test_utils.h"
#include "gtest/gtest.h"

TEST(GTest, GameSetsUp)
{
    auto test_factory = CreateApplicationTestFactory(
        [](chrono::milliseconds delta_time, ApplicationWindow& window) {},
        [](chrono::milliseconds delta_time, Application& application)
        { application.Close_Application(); });
    Create_Application(move(test_factory), true);
}
