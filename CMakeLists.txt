
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
set(CMAKE_CXX_STANDARD 23)

add_executable(BoarCastle
    src/main.cpp
    src/handlers/time_handler.cpp
    src/handlers/input_handler.cpp
    src/managers/camera.cpp
    src/managers/construction_manager.cpp
    src/managers/collision_manager.cpp
    src/managers/unit_manager.cpp
    src/managers/pathfinder.cpp
    src/game_world/game_objects/wall.cpp
    src/game_world/game_objects/worker.cpp
    src/game_world/world.cpp
)

# libraries

find_package(raylib CONFIG REQUIRED)
target_link_libraries(BoarCastle PRIVATE raylib)

target_link_libraries(BoarCastle PRIVATE -fsanitize=address -fsanitize=undefined)
target_compile_options(BoarCastle PRIVATE -std=c++17 -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused )

# optimized:
# target_compile_options(BoarCastle PRIVATE -O3)
# debug:
target_compile_options(BoarCastle PRIVATE -g)