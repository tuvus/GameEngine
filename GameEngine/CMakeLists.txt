cmake_minimum_required(VERSION 3.25)
project(GameEngine VERSION 0.1.0 DESCRIPTION "GameEngine for C++ games" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(Source
    src/library.cpp
)

set(Headers
    include/GameEngine/library.h
)

include_directories(include/GameEngine)

add_library(${PROJECT_NAME} STATIC ${Sources} ${Headers})
set_target_properties(GameEngine PROPERTIES LINKER_LANGUAGE CXX)