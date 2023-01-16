#include "worker.hpp"

#include <cstdint>
#include <iostream>
#include <raylib.h>

#include "../world.hpp"
#include "../../utils/utils.hpp"

Worker::Worker(const boar::IndexVector2 pos)
    :index{pos}, step_target{pos}, target{pos}, render_pos{(float)pos.x, 0.5f, (float)pos.z}
{
    game_world.get_tile(this->index)->empty = false;
}

void Worker::move_to(const boar::IndexVector2 target)
{
    if(target == this->target || target == this->index) return;

    TimeMeasurer worker{"found worker path in "};
    if(moving)
    {
        auto new_path = game_world.get_path(this->step_target, target);
        this->path = new_path;
    }
    else 
    {
        this->path = game_world.get_path(this->index, target);
    }

    worker.print_time();
    
    if(!this->path.empty())
    {
        this->target = target;
        this->moving = true;
    }
}

void Worker::update(const float delta)
{
    if(this->moving)
    {
        if(this->step_target == this->index)
        {
            this->step_target = this->path.back();
            this->path.pop_back();
        }
        
        const boar::IndexVector2 dir = this->step_target - this->index;
        const float movement_cost = game_world.get_movement_cost(dir);

        this->step_progress += delta * Worker::SPEED / movement_cost ;
        if(this->step_progress > 1)
        {
            game_world.get_tile(this->index)->empty = true;
            game_world.get_tile(this->step_target)->empty = false;
            this->step_progress -= 1;
            this->index = this->step_target;
            if(this->index == this->target)
            {
                this->moving = false;
                this->step_progress = 0;
            }
            else 
            {
                this->step_target = this->path.back();
                this->path.pop_back();

            }
        }

        this->render_pos = boar::Vector3f
        {
            this->index.x + 0.5f + (dir.x * step_progress) ,
            0.5f,
            this->index.z + 0.5f + (dir.z * step_progress)
        };
    }
}

void Worker::render() const
{
    DrawSphere(render_pos, 0.5, this->color);
}