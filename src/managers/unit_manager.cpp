// header
#include "unit_manager.hpp"

// builtin
#include <cstdint>
#include <iostream>

// extern
#include "raylib.h"

// local
#include "../game_world/world.hpp"
#include "../utils/utils.hpp"
#include "collision_manager.hpp"
#include "pathfinder.hpp"


UnitManager::UnitManager()
{
    this->workers.emplace_back(boar::IndexVector2{20, 20});
}

void UnitManager::update(const float delta, const InputData& input_data)
{
    for (Worker& worker: this->workers)
    {
        worker.update(delta);
    }

    if (game_world.current_input_mode != World::InputMode::COMMAND)
        return;

    const bool mouse_inside_borders = game_world.collision_manager->is_inside_borders(input_data.mouse_index);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && mouse_inside_borders)
    {
        this->workers[0].move_to(input_data.mouse_index);
    }

    // TODO: Remove pathfinder benchmark from here
    if (IsKeyDown('T'))
    {
        auto total_time = 0;
        std::cout << "PF test\n";
        boar::IndexVector2 origin{0, 0};
        for (int32_t x = 0; x < 300; x += 30)
        {
            for (int32_t z = 0; z < 300; z += 30)
            {
                boar::IndexVector2 target{x, z};
                std::cout << target;
                TimeMeasurer pathtimer{"Path found in"};
                this->path = game_world.pathfinder->get_path(origin, target);
                total_time += pathtimer.get_time();
                pathtimer.print_time();
            }
        }
        std::cout << "Found all 100 paths in " << total_time / 1000 << "." << total_time % 1000 << "s\n";
    }
}

void UnitManager::render() const
{
    for (const auto& tile: this->workers[0].path)
    {
        Vector3 pos{};
        pos.x = tile.x + 0.5;
        pos.y = 0;
        pos.z = tile.z + 0.5;
        DrawCube(pos, 1, 0.1, 1, GREEN);
    }

    for (const Worker& worker: this->workers)
    {
        worker.render();
    }
}