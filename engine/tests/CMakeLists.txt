find_package(GTest REQUIRED)
enable_testing()

add_executable(Test
        ${Headers}
        application_factory_test_utils.h
        application_tests.cpp
        network_tests.cpp
)
target_link_libraries(Test PRIVATE GTest::gtest_main engine)
gtest_discover_tests(Test)


