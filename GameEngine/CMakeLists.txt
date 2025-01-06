cmake_minimum_required(VERSION 3.25)
project(GameEngine VERSION 0.1.0 DESCRIPTION "GameEngine for C++ games" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

set(Sources
        src/Game.cpp
)

set(Headers
        ${PROJECT_SOURCE_DIR}/include/GameEngine/Game.h
)


add_library(${PROJECT_NAME} STATIC ${Sources} ${Headers})
set_target_properties(GameEngine PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(GameEngine PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/include/GameEngine ${PROJECT_SOURCE_DIR})

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
target_link_libraries(GameEngine PUBLIC glfw glad::glad imgui::imgui)

if (NOT DONT_RUN_GAME_ENGINE_TESTS)
    add_subdirectory(tests)
endif ()