find_package(GTest REQUIRED)
enable_testing()

add_executable(Test
        ${Headers}
        ApplicationFactoryTestUtils.h
        ApplicationTests.cpp
        NetworkTests.cpp)
target_link_libraries(Test PRIVATE GTest::gtest_main GameEngine)
gtest_discover_tests(Test)


