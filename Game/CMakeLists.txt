cmake_minimum_required(VERSION 3.25)
project(Game VERSION 0.1.0)

set(CMAKE_TOOLCHAIN_FILE, "${CMAKE_SOURCE_DIR}/vcpkg.cmake")

set(Sources
        src/CardGame.cpp
        src/main.cpp
        src/Lobby_Scene.cpp
)

set(Headers
        include/CardGame.h
        include/Lobby_Scene.h
        include/Menu_Scene.h
        include/Game_Scene.h
)

set(CMAKE_CXX_STANDARD 23)
add_executable(Game ${Sources} ${Headers})

#set(DONT_RUN_GAME_ENGINE_TESTS TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works
add_subdirectory(${CMAKE_SOURCE_DIR}/../GameEngine ${CMAKE_CURRENT_BINARY_DIR}/GameEngine)
target_link_libraries(Game PUBLIC GameEngine)
target_include_directories(Game PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Copy compile_commands.json after build config
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_SOURCE_DIR}/compile_commands.json"
)
