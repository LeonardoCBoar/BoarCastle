// header
#include "unit_manager.hpp"

// builtin
#include <algorithm>
#include <cstdint>
#include <iostream>

// extern
#include "raylib.h"

// local
#include "../game_world/world.hpp"
#include "../properties/colors.hpp"
#include "../utils/utils.hpp"
#include "collision_manager.hpp"
#include "pathfinder.hpp"


UnitManager::UnitManager()
{
    this->workers.emplace_back(boar::IndexVector2{20, 20}, this->next_worker_id++);
}

void UnitManager::update(const float delta, const InputData& input_data)
{
    for (Worker& worker: this->workers)
    {
        worker.update(delta);
    }

    if (game_world.current_input_mode != World::InputMode::COMMAND)
        return;

    this->handle_input(input_data);

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
                const auto path = game_world.pathfinder->get_path(origin, target);
                total_time += pathtimer.get_time();
                pathtimer.print_time();
            }
        }
        std::cout << "Found all 100 paths in " << total_time / 1000 << "." << total_time % 1000 << "s\n";
    }
}

void UnitManager::handle_input(const InputData& input_data)
{
    const MouseInput mouse_input = input_data.mouse_input;
    const auto mouse_index = mouse_input.index;

    this->selection_rect = std::nullopt;

    switch (mouse_input.type)
    {
        case MouseInputType::LEFT_CLICK:
        {
            auto is_worker_under_mouse = [mouse_index](const auto& worker) { return worker.index == mouse_index; };
            auto clicked_worker = std::find_if(this->workers.begin(), this->workers.end(), is_worker_under_mouse);
            if (clicked_worker != this->workers.end())
            {
                if (this->selected_worker_id.has_value())
                    this->get_selected_worker()->selected = false;

                clicked_worker->selected = true;
                this->selected_worker_id = clicked_worker->id;
            }
            break;
        }
        case MouseInputType::SHIFT_LEFT_CLICK:
        {
            if (game_world.collision_manager->can_move_to_tile(mouse_index))
                this->workers.emplace_back(mouse_index, this->next_worker_id++);
            break;
        }
        case MouseInputType::LEFT_CLICK_SELECTION:
        {
            this->selection_rect = std::make_optional<Rect>(
                Rect::from_unoredered_points(mouse_input.selection_start_pos.x, mouse_input.selection_start_pos.z,
                                             mouse_input.pos.x, mouse_input.pos.z));

            const Rect selection_rect_index = Rect::from_unoredered_points(
                mouse_input.selection_start_index.x, mouse_input.selection_start_index.z, mouse_index.x, mouse_index.z);

            this->selected_workers.clear();
            if (selection_rect_index.size_x > 0 || selection_rect_index.size_z > 0)
            {
                for (auto& worker: this->workers)
                {
                    worker.selected = worker.index.is_inside_rect(selection_rect_index);
                    if(worker.selected)
                        this->selected_workers.push_back(worker.id);
                }
            }
            break;
        }
        case MouseInputType::RIGHT_CLICK:
        {
            uint64_t selected_worker_id;
            int32_t k = 0;
            int32_t unassigned_units = this->workers.size();

            while(true)
            {
                std::cout << "abc\n";
                for(int32_t x = -k; x <= k; x++)
                {
                    const int32_t z = k + 1;
                    Worker* unit = &this->workers[unassigned_units - 1];
                    const boar::IndexVector2 unit_target{mouse_index.x + x, mouse_index.z + z};
                    if(!game_world.collision_manager->is_inside_borders(unit_target))
                    {
                        std::cout << unit_target << std::endl;
                        std::cout <<"www\n";
                        continue;
                    }
                    
                    unit->move_to(unit_target);
                    std::cout << "target for: " << unit->id << " is " << unit_target;

                    unassigned_units--;

                    if(unassigned_units == 0)
                        goto after;
                }
                k++;
                std::cout << k << std::endl;
                if(unassigned_units == 0)
                    goto after;
                if(k > 10)
                {
                    std::cout << "FAIL\n";
                    return;

                }
            }

            after:
                break;
        }
        case MouseInputType::MI_NONE:
        {
            break;
        }
        default:
        {
            assert(false);
        }
    }
}

void UnitManager::render_3d() const
{
    if (this->selected_worker_id.has_value())
    {
        for (const auto& tile: this->get_selected_worker()->path)
        {
            Vector3 pos{};
            pos.x = tile.x + 0.5;
            pos.y = 0;
            pos.z = tile.z + 0.5;
            DrawCube(pos, 1, 0.1, 1, GREEN);
        }
    }

    for (const Worker& worker: this->workers)
    {
        worker.render();
    }
}

void UnitManager::render_2d() const
{
    if (this->selection_rect.has_value())
    {
        DrawRectangle(this->selection_rect->pos_x, this->selection_rect->pos_z, this->selection_rect->size_x,
                      this->selection_rect->size_z, property::color.SELECTION_AREA_COLOR);
    }
}

Worker* UnitManager::get_selected_worker()
{
    return &this->workers[this->selected_worker_id.value()];
}

const Worker* UnitManager::get_selected_worker() const
{
    return &this->workers[this->selected_worker_id.value()];
}