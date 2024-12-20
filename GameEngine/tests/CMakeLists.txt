find_package(GTest)
enable_testing()

add_executable(Test GameTests.cpp ${Headers})
target_link_libraries(Test GTest::gtest_main GameEngine)
gtest_discover_tests(Test)


