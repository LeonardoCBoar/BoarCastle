cmake_minimum_required(VERSION 3.0.0)

project(BoarCastle)

add_executable( BoarCastle
    src/main.cpp
    src/camera.cpp
    #src/interaction_modules/construction_manager.cpp
)
target_include_directories(BoarCastle PRIVATE raylib/src/)
target_link_libraries(BoarCastle PRIVATE -fsanitize=address)
target_link_libraries(BoarCastle PRIVATE raylib )


