// header
#include "construction_manager.hpp"

// builtin
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <ostream>
#include <stack>
#include <type_traits>

// extern
#include <raylib.h>

// local
#include "../game_world/game_objects/wall.hpp"
#include "../game_world/game_objects/worker.hpp"
#include "../game_world/world.hpp"
#include "collision_manager.hpp"
#include "unit_manager.hpp"



ConstructionManager::ConstructionManager()
{
    this->create_preview_wall();
}

void ConstructionManager::create_preview_wall(boar::IndexVector2 const mouse_index)
{
    this->preview_wall = std::make_shared<Wall>(mouse_index);
    this->preview_wall->color.a = 125;
}

void ConstructionManager::assign_order(ConstructionOrder& order)
{
    // TODO: Select best worker
    for (auto& worker: game_world.unit_manager->workers)
    {
        if (worker.target_construction == NULL)
        {
            order.state = IN_CONSTRUCTION;
            worker.target_construction = &order;
            break;
        }
    }
}

void ConstructionManager::update(const InputData& input_data)
{
    this->handle_input(input_data);

    std::stack<size_t> indexes_to_remove;

    for (auto& [index, order]: this->construction_queue)
    {
        switch (order.state)
        {
            case NOT_ASSIGNED:
                assign_order(order);
                break;
            case IN_CONSTRUCTION:
                break;
            case INACESSIBLE:
                // TODO: Logic to check again inacessible constructions
                break;
            case FINISHED:
                game_world.walls.push_back(order.construction);
                order.construction->color.a = MAX_CONSTRUCTION_TRANSPARENCY;
                this->construction_queue.erase(index);
                break;
            default:
                assert(false);
        }
    }
}

void ConstructionManager::handle_input(const InputData& input_data)
{
    if (game_world.current_input_mode != World::InputMode::CONSTRUCTION)
        return;

    const auto selected_tile = input_data.mouse_index;

    if (!game_world.collision_manager->is_inside_borders(selected_tile))
    {
        this->preview_wall->visible = false;
        return;
    }

    this->preview_wall->visible = true;
    this->preview_wall->move_to(selected_tile);


    bool free_space = game_world.collision_manager->can_fit_object(preview_wall);


    if (free_space)
    {
        preview_wall->color = GRAY;
        preview_wall->color.a = 125;
    }
    else
    {
        preview_wall->color = RED;
        preview_wall->color.a = 255;
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && free_space)
    {
        game_world.collision_manager->add_object_collision(this->preview_wall);
        this->construction_queue[this->order_id++] = {0, this->preview_wall};
        this->create_preview_wall(selected_tile);
    }
}

void ConstructionManager::render() const
{
    for (const auto& order: this->construction_queue)
    {
        order.second.construction->color.a =
            MIN_CONSTRUCTION_TRANSPARENCY +
            (MAX_CONSTRUCTION_TRANSPARENCY - MIN_CONSTRUCTION_TRANSPARENCY) * order.second.progress;
        order.second.construction->render();
        const auto interaction_spots = order.second.construction->get_interaction_spots();

        for (const auto& spot: interaction_spots)
        {
            if (game_world.collision_manager->is_inside_borders(spot) &&
                game_world.collision_manager->is_tile_empty(spot))
            {
                DrawCube({spot.x + 0.5f, 0, spot.z + 0.5f}, 1, 0.1, 1, YELLOW);
            }
        }
    }

    if (game_world.current_input_mode != World::InputMode::CONSTRUCTION)
        return;
    // Preview Wall
    this->preview_wall->render();
}