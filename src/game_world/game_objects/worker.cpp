// header
#include "worker.hpp"

// builtin
#include <cstdint>
#include <iostream>

// extern
#include <raylib.h>

// local
#include "../../utils/utils.hpp"
#include "../world.hpp"



Worker::Worker(boar::IndexVector2 const pos):
    index{pos}, step_target{pos}, target{pos}, render_pos{(float)pos.x, 0.5f, (float)pos.z}
{
    game_world.get_tile(this->index)->empty = false;
}

void Worker::move_to(boar::IndexVector2 const target)
{
    if (target == this->target || target == this->index)
        return;

    TimeMeasurer worker{"found worker path in "};
    if (moving) {
        auto new_path = game_world.get_path(this->step_target, target);
        this->path = new_path;
    }
    else {
        this->path = game_world.get_path(this->index, target);
    }

    worker.print_time();

    if (!this->path.empty()) {
        this->target = target;
        this->moving = true;
    }
}

void Worker::update_movement(float const delta)
{

    auto move_next_tile = [this] {
        auto get_next_tile = [this] {
            this->step_target = this->path.back();
            this->path.pop_back();
        };

        if (!game_world.is_tile_empty(this->step_target)) {

            do {
                get_next_tile();
            } while (!game_world.is_tile_empty(this->step_target));

            auto outline_path = game_world.get_path(this->index, this->step_target);
            this->path.insert(this->path.end(), outline_path.begin(), outline_path.end());
            get_next_tile();
        }
    };


    if (this->step_target == this->index) {
        move_next_tile();
    }

    boar::IndexVector2 const dir = this->step_target - this->index;
    float const movement_cost = game_world.get_movement_cost(dir);

    this->step_progress += delta * Worker::SPEED / movement_cost;
    if (this->step_progress > 1) {
        game_world.get_tile(this->index)->empty = true;
        game_world.get_tile(this->step_target)->empty = false;
        this->step_progress -= 1;
        this->index = this->step_target;
        if (this->index == this->target) {
            this->moving = false;
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
    if (this->moving)
        this->update_movement(delta);
}

void Worker::render() const
{
    DrawSphere(render_pos, 0.5, this->color);
}