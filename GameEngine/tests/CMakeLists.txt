find_package(GTest REQUIRED)
enable_testing()

add_executable(Test GameTests.cpp ${Headers})
target_link_libraries(Test PRIVATE GTest::gtest_main GameEngine)
gtest_discover_tests(Test)


