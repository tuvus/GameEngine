cmake_minimum_required(VERSION 3.25)
project(Game VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
add_executable(Game main.cpp)
add_subdirectory(${CMAKE_SOURCE_DIR}/../GameEngine ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(Game PUBLIC ${CMAKE_SOURCE_DIR}/lib/CmakeFiles/GameEngine)
#target_include_directories(Game PUBLIC ${CMAKE_SOURCE_DIR}/lib/CMakeFiles/GameEngine.dir)