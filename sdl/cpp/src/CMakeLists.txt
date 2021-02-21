# Build file
cmake_minimum_required(VERSION 3.10)

# set the project name
project(TankGame LANGUAGES CXX C VERSION 0.0.1)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

# add the executable
add_executable(TankGame main.cpp)
set_target_properties(TankGame PROPERTIES OUTPUT_NAME "tankgame")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_library(MediaLib media.cpp)
add_library(UILib ui.cpp)
#add_library(SceneLib scenes/main_scene.hpp)
configure_file(config.h.in config.h)

target_include_directories(TankGame PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(TankGame PUBLIC ${SDL2IMAGE_INCLUDE_DIRS})
target_include_directories(TankGame PUBLIC ${SDL2TTF_INCLUDE_DIRS})

target_link_libraries(TankGame PUBLIC MediaLib)
target_link_libraries(TankGame PUBLIC UILib)
#target_link_libraries(TankGame PUBLIC SceneLib)
target_link_libraries(TankGame PUBLIC ${SDL2_LIBRARIES})
target_link_libraries(TankGame PUBLIC ${SDL2TTF_LIBRARIES})
target_link_libraries(TankGame PUBLIC ${SDL2IMAGE_LIBRARIES})

target_include_directories(TankGame PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(TankGame PUBLIC "${PROJECT_SOURCE_DIR}")

message("${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}")
