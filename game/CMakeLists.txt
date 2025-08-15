cmake_minimum_required(VERSION 3.25)
project(Game VERSION 0.1.0)

set(CMAKE_TOOLCHAIN_FILE, "${CMAKE_SOURCE_DIR}/vcpkg.cmake")

set(Sources
        src/card_game.cpp
        src/main.cpp
        src/lobby_scene.cpp
)

set(Headers
        include/card_game.h
        include/lobby_scene.h
        include/menu_scene.h
        include/game_scene.h
)

set(CMAKE_CXX_STANDARD 23)
add_executable(Game ${Sources} ${Headers})

#set(DONT_RUN_GAME_ENGINE_TESTS TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works
add_subdirectory(${CMAKE_SOURCE_DIR}/../engine ${CMAKE_CURRENT_BINARY_DIR}/engine)
target_link_libraries(Game PUBLIC euclid)
target_include_directories(Game PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Copy compile_commands.json after build config
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/compile_commands.json"
        "${CMAKE_SOURCE_DIR}/compile_commands.json"
)
