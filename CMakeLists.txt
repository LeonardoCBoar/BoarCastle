cmake_minimum_required(VERSION 3.0.0)

project(BoarCastle)

add_executable( BoarCastle
    src/main.cpp
    src/input_modules/camera.cpp
    src/input_modules/construction_manager.cpp
    src/input_modules/unit_manager.cpp
    src/game_world/game_objects/wall.cpp
    src/game_world/world.cpp
)
target_include_directories(BoarCastle PRIVATE raylib/src/)
target_link_libraries(BoarCastle PRIVATE -fsanitize=address)
target_link_libraries(BoarCastle PRIVATE raylib )

