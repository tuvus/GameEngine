cmake_minimum_required(VERSION 3.25)
project(Game VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_TOOLCHAIN_FILE, "${CMAKE_SOURCE_DIR}/vcpkg.cmake")

set(Sources
        src/CardGame.cpp
        src/main.cpp
)

set(Headers
        include/CardGame.h
)

set(CMAKE_CXX_STANDARD 23)
add_executable(Game ${Sources} ${Headers})

#set(DONT_RUN_GAME_ENGINE_TESTS TRUE)
add_subdirectory(${CMAKE_SOURCE_DIR}/../GameEngine ${CMAKE_CURRENT_BINARY_DIR}/GameEngine)
target_link_libraries(Game PUBLIC GameEngine)
target_include_directories(Game PRIVATE ${PROJECT_SOURCE_DIR}/include)
