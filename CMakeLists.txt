cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# set the project name
project(GameOfLife)

# setup project
include_directories(include)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# use sdl
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# add GameEngine
add_library(GameEngine INTERFACE)
target_include_directories(GameEngine INTERFACE .)

# make executable
add_executable(GameOfLife src/main.cpp)
target_link_libraries(GameOfLife ${SDL2_LIBRARIES} SDL2::TTF)