#include "worker.hpp"

#include <raylib.h>

#include "../world.hpp"

void Worker::move_to(const boar::IndexVector2 target)
{
    this->index = target;
    this->render_pos = boar::Vector3f
    {
        (float)this->index.x + 0.5f,
        0.5f, 
        (float)this->index.z + 0.5f
    };
}

void Worker::update(const float delta)
{

}

void Worker::render() const
{
    DrawSphere(render_pos, 1, this->color);
}