// header
#include "worker.hpp"

// builtin
#include <assert.h>
#include <cstdint>
#include <iostream>

// extern
#include <raylib.h>

// local
#include "../../managers/collision_manager.hpp"
#include "../../managers/construction_manager.hpp"
#include "../../managers/pathfinder.hpp"
#include "../../properties/colors.hpp"
#include "../../utils/utils.hpp"
#include "../world.hpp"
#include "wall.hpp"

Worker::Worker(boar::IndexVector2 const pos, const uint64_t id):
    id{id}, index{pos}, step_target{pos}, target{pos}, render_pos{static_cast<float>(pos.x), 0.5f,
                                                                  static_cast<float>(pos.z)}
{
    game_world.get_tile(this->index)->empty = false;
}

bool Worker::move_to(boar::IndexVector2 const target)
{
    if (target == this->target || target == this->index)
        return false;

    TimeMeasurer worker{"found worker path in "};

    Path new_path;
    if (current_state == Worker::MOVING)
    {
        new_path = game_world.pathfinder->get_path(this->step_target, target);
    }
    else
    {
        new_path = game_world.pathfinder->get_path(this->index, target);
    }


    worker.print_time();

    if (new_path.empty())
        return false;

    this->path = new_path;
    this->target = target;
    if (this->current_state != Worker::MOVING)
    {
        this->step_target = this->pop_next_movement();
        current_state = Worker::MOVING;
    }
    return true;
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

boar::IndexVector2 Worker::pop_next_movement()
{
    assert(this->path.size() > 0);
    auto next_movement = this->path.back();
    this->path.pop_back();
    return next_movement;
}

bool Worker::try_move_next_tile()
{
    if (!game_world.collision_manager->is_tile_empty(this->target))
        return false;


    this->step_target = this->pop_next_movement();
    if (game_world.collision_manager->is_tile_empty(this->step_target))
        return true;

    do
    {
        if (!this->path.empty())
        {
            this->step_target = this->pop_next_movement();
        }
        else
            return false;

    } while (!game_world.collision_manager->is_tile_empty(this->step_target));

    auto outline_path = game_world.pathfinder->get_path(this->index, this->step_target);
    if (outline_path.empty())
        return false;

    this->path.insert(this->path.end(), outline_path.begin(), outline_path.end());
    this->step_target = this->pop_next_movement();

    return true;
}

void Worker::update_movement(const float delta)
{
    boar::IndexVector2 const dir = this->step_target - this->index;
    assert(this->step_target != this->index);
    assert(dir.x <= 1);
    assert(dir.z <= 1);

    const float movement_cost = Pathfinder::get_distance_cost(dir);

    this->step_progress += delta * Worker::MOVE_SPEED / movement_cost;
    while (this->step_progress > 1)
    {
        // TODO: step target must be reserved at the begining of the movement
        game_world.get_tile(this->index)->empty = true;
        game_world.get_tile(this->step_target)->empty = false;
        this->step_progress -= 1;
        this->index = this->step_target;
        if (this->index == this->target)
        {
            this->current_state = IDLE;
            this->step_progress = 0;
        }
        else
        {
            const bool can_continue_moving = try_move_next_tile();
            if (!can_continue_moving)
            {
                if (this->target_construction != nullptr)
                    this->close_current_order(INACESSIBLE);

                this->current_state = IDLE;
                this->step_progress = 0;
            }
        }
    }

    // const double normalized_step = step_progress > 1 ? 1 : this->step_progress;
    this->render_pos = boar::Vector3f{this->index.x + 0.5f + (dir.x * step_progress), 0.5f,
                                      this->index.z + 0.5f + (dir.z * step_progress)};
}


void Worker::update(const float delta)
{
    switch (this->current_state)
    {
        case IDLE:
            if (this->target_construction != nullptr)
            {
                auto interaction_spots = this->target_construction->construction->get_interaction_spots();

                bool can_build_range = false;
                for (const auto& spot: interaction_spots)
                {
                    if (!game_world.collision_manager->is_inside_borders(spot))
                        continue;

                    if (this->index == spot)
                    {
                        can_build_range = true;
                        break;
                    }
                }

                if (can_build_range)
                {
                    this->current_state = BUILDING;
                }
                else
                {
                    bool found_path;
                    for (const auto& spot: interaction_spots)
                    {
                        if (!game_world.collision_manager->is_inside_borders(spot) ||
                            !game_world.collision_manager->is_tile_empty(spot))
                            continue;

                        found_path = this->move_to(spot);
                        if (found_path)
                        {
                            break;
                        }
                    }
                    if (!found_path)
                    {
                        this->close_current_order(INACESSIBLE);
                    }
                }
            }
            break;
        case MOVING:
            this->update_movement(delta);
            break;

        case BUILDING:
            this->target_construction->progress += BUILD_SPEED * delta;
            if (this->target_construction->progress >= 1.0f)
            {
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
    Color color;
    if (this->selected)
        color = property::color.SELECTED_WORKER_COLOR;
    else
        color = property::color.WORKER_COLOR;

    DrawSphere(render_pos, 0.5, color);
}