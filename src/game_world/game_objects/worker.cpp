// header
#include "worker.hpp"

// builtin
#include <cstdint>
#include <iostream>
#include <assert.h>
#include <optional>

// extern
#include <raylib.h>

// local
#include "../../utils/utils.hpp"
#include "../world.hpp"
#include "wall.hpp"
#include "../../input_modules/construction_manager.hpp"

Worker::Worker(boar::IndexVector2 const pos):
    index{pos}, step_target{pos}, target{pos}, render_pos{(float)pos.x, 0.5f, (float)pos.z}
{
    game_world.get_tile(this->index)->empty = false;
}

bool Worker::move_to(boar::IndexVector2 const target)
{
    if (target == this->target || target == this->index)
        return false;

    TimeMeasurer worker{"found worker path in "};
    if (current_state == Worker::MOVING) {
        auto new_path = game_world.get_path(this->step_target, target);
        this->path = new_path;
    }
    else {
        this->path = game_world.get_path(this->index, target);
    }

    worker.print_time();

    if (!this->path.empty()) {
        this->target = target;
        current_state = Worker::MOVING;
        return true;
    }

    return false;
}

void Worker::construct(ConstructionOrder* target_construction)
{
    this->target_construction = target_construction;
}

void Worker::close_current_order(const ConstructionOrderState close_state)
{
    this->target_construction->state = close_state;
    this->target_construction = nullptr;
}

void Worker::update_movement(float const delta)
{
    //TODO: Worker cannot recover if there's no path to the target when he's in the middle of it

    auto move_next_tile = [this]
    {
        auto get_next_tile = [this] 
        {
            bool equal_zero = this->path.size() == 0;
            this->step_target = this->path.back(); //TODO: Handle case when theres is no more empty paths
            this->path.pop_back();
            if(equal_zero)
                std::cout << "back when size == 0 " << this->step_target;
        };

        if (!game_world.is_tile_empty(this->step_target)) {

            do
            {
                get_next_tile();
            } 
            while (!game_world.is_tile_empty(this->step_target)); //TODO: There's a crash here

            auto outline_path = game_world.get_path(this->index, this->step_target);
            std::cout << "Outline path size: " << outline_path.size() << "\n"; //TODO: Review print always being called
            this->path.insert(this->path.end(), outline_path.begin(), outline_path.end());
            get_next_tile();
        }
    };


    if (this->step_target == this->index) {
        move_next_tile();
    }

    boar::IndexVector2 const dir = this->step_target - this->index;
    float const movement_cost = game_world.get_movement_cost(dir);

    this->step_progress += delta * Worker::MOVE_SPEED / movement_cost;
    if (this->step_progress > 1) {
        game_world.get_tile(this->index)->empty = true;
        game_world.get_tile(this->step_target)->empty = false;
        this->step_progress -= 1;
        this->index = this->step_target;
        if (this->index == this->target) {
            this->current_state = IDLE;
            this->step_progress = 0;
        }
        else {
            move_next_tile();
        }
    }

    this->render_pos = boar::Vector3f{this->index.x + 0.5f + (dir.x * step_progress), 0.5f,
                                      this->index.z + 0.5f + (dir.z * step_progress)};
}


void Worker::update(float const delta)
{
    // std::cout << "Current State: " << this->current_state << std::endl;
    switch (this->current_state)
    {
    case IDLE:
        if(this->target_construction != nullptr)
        {
            auto interaction_spots =  this->target_construction->construction->get_interaction_spots();
            std::optional<boar::IndexVector2> possible_interaction_spot = std::nullopt;
            bool can_build_range = false;

            for(const auto& spot : interaction_spots)
            {
                if(!game_world.is_inside_borders(spot))
                    continue;

                if(this->index == spot)
                {
                    can_build_range = true;
                    break;
                }
                else if(game_world.is_tile_empty(spot))
                    possible_interaction_spot = spot;

            }

            //TODO: Handle no path case
            if(can_build_range)
                this->current_state = BUILDING;
            else if(possible_interaction_spot.has_value())
            {
                const bool found_path = this->move_to(possible_interaction_spot.value());
                if(!found_path)
                {
                    std::cout << "No path found to building\n";
                    this->close_current_order(INACESSIBLE);
                }
            }
            else
            {
                std::cout << "Building cannot be accessed\n";
                this->close_current_order(INACESSIBLE);
            }

        }
        break;
    case MOVING:
        this->update_movement(delta);
        break;
    
    case BUILDING:
        this->target_construction->progress += BUILD_SPEED * delta;
        if(this->target_construction->progress >= 1.0f)
        {
            std::cout << "Finished construction!!!\n";
            this->close_current_order(FINISHED);
            this->current_state = IDLE;
        }
        break;
    
    default:
        break;
    }
}

void Worker::render() const
{
    DrawSphere(render_pos, 0.5, this->color);
}