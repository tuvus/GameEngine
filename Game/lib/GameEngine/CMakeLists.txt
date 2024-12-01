cmake_minimum_required(VERSION 3.25)
project(GameEngine VERSION 0.1.0 DESCRIPTION "GameEngine for C++ games" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(Source
        library.cpp
)

set(Headers
        library.h
)

add_library(${PROJECT_NAME} STATIC ${Source} ${Headers})
set_target_properties(GameEngine PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(GameEngine PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/include/GameEngine ${PROJECT_SOURCE_DIR})
