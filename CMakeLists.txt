
# cmake config

cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_TOOLCHAIN_FILE ./vcpkg/scripts/buildsystems/vcpkg.cmake CACHE PATH "vcpkg toolchain path")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# project config

project(BoarCastle CXX)

set(CMAKE_CXX_FLAGS_DEBUG "" CACHE STRING "flags")
set(CMAKE_CXX_FLAGS_RELEASE  "" CACHE STRING "flags")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "" CACHE STRING "flags")
set(CMAKE_CXX_FLAGS_MINSIZEREL  "" CACHE STRING "flags")
set(CMAKE_CXX_STANDARD 17)

add_executable(BoarCastle
    src/main.cpp
    src/managers/camera.cpp
    src/managers/construction_manager.cpp
    src/managers/unit_manager.cpp
    src/game_world/game_objects/wall.cpp
    src/game_world/game_objects/worker.cpp
    src/game_world/world.cpp
)

# libraries

find_package(raylib CONFIG REQUIRED)
target_link_libraries(BoarCastle PRIVATE raylib)

target_link_libraries(BoarCastle PRIVATE -fsanitize=address -fsanitize=undefined)
target_compile_options(BoarCastle PRIVATE -std=c++17 -pedantic -Wall -Wextra)

# optimized:
# target_compile_options(BoarCastle PRIVATE -O3)
# debug:
target_compile_options(BoarCastle PRIVATE -g)