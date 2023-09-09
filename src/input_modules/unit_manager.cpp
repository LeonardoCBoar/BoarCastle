// header
#include "unit_manager.hpp"

// builtin
#include <cstdint>
#include <iostream>

// extern
#include "raylib.h"

// local
#include "../game_world/world.hpp"
#include "../input_modules/camera.hpp"
#include "../utils/utils.hpp"



UnitManager::UnitManager(HoverCamera const* const camera): camera{camera}
{
    this->workers.emplace_back(boar::IndexVector2{20, 20});
}

void UnitManager::update(float const delta)
{

    for (Worker& worker: this->workers) {
        worker.update(delta);
    }

    if (game_world.current_input_mode != World::InputMode::COMMAND)
        return;

    bool const is_inside_borders = game_world.is_inside_borders(camera->current_mouse_index);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && is_inside_borders) {
        this->workers[0].move_to(camera->current_mouse_index);
    }

    if (IsKeyDown('T')) {
        auto total_time = 0;
        std::cout << "PF test\n";
        boar::IndexVector2 origin{0, 0};
        for (int32_t x = 0; x < 300; x += 30) {

            for (int32_t z = 0; z < 300; z += 30) {
                boar::IndexVector2 target{x, z};
                std::cout << target;
                TimeMeasurer pathtimer{"Path found in"};
                this->path = game_world.get_path(origin, target);
                total_time += pathtimer.get_time();
                pathtimer.print_time();
            }
        }
        std::cout << "Found all 100 paths in " << total_time / 1000 << "." << total_time % 1000 << "s\n";
    }
}

void UnitManager::render() const
{

    for (auto const& tile: this->workers[0].path) {
        Vector3 pos{};
        pos.x = tile.x + 0.5;
        pos.y = 0;
        pos.z = tile.z + 0.5;
        DrawCube(pos, 1, 0.1, 1, GREEN);
    }

    for (Worker const& worker: this->workers) {
        worker.render();
    }
}