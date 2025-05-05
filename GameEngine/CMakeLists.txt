cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME GameEngine)
project(${PROJECT_NAME} VERSION 0.1.0 DESCRIPTION "GameEngine for C++ games" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

set(Sources
        src/Application.cpp
        src/networking/Network.cpp
        src/ui/button.cpp
        src/ui/container.cpp
        src/ui/context.cpp
        src/ui/element.cpp
)

set(Headers
        ${PROJECT_SOURCE_DIR}/include/GameEngine/Application.h
        ${PROJECT_SOURCE_DIR}/include/GameEngine/ApplicationFactory.h
        ${PROJECT_SOURCE_DIR}/include/GameEngine/networking/Network.h
        ${PROJECT_SOURCE_DIR}/include/GameEngine/ui/eui.h
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works
add_library(${PROJECT_NAME} STATIC ${Sources} ${Headers})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/include/GameEngine ${PROJECT_SOURCE_DIR})

# find_package(glfw3 CONFIG REQUIRED)
# find_package(glad CONFIG REQUIRED)
# find_package(imgui CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad::glad imgui::imgui GameNetworkingSockets::static GameNetworkingSockets::GameNetworkingSockets)
find_package(raylib CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib GameNetworkingSockets::static GameNetworkingSockets::GameNetworkingSockets)
find_package(GameNetworkingSockets CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GameNetworkingSockets::static GameNetworkingSockets::GameNetworkingSockets)

# if (NOT DONT_RUN_GAME_ENGINE_TESTS)
#     add_subdirectory(tests)
# endif ()

# Copy compile_commands.json after build config
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_SOURCE_DIR}/compile_commands.json"
)
