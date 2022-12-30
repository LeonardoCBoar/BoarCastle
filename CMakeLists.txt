cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(BoarCastle CXX)
add_executable(BoarCastle
    src/main.cpp
    src/input_modules/camera.cpp
    src/input_modules/construction_manager.cpp
    src/input_modules/unit_manager.cpp
    src/game_world/game_objects/wall.cpp
    src/game_world/world.cpp
)

find_package(raylib CONFIG REQUIRED)

target_link_libraries(BoarCastle PRIVATE raylib -fsanitize=address -fsanitize=undefined)
target_compile_options(BoarCastle PRIVATE -std=c++17 -g)
