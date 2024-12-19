
find_package(GTest)
enable_testing()

add_executable(Test GameTests.cpp)
target_link_libraries(Test GTest::gtest_main)
gtest_discover_tests(Test)


