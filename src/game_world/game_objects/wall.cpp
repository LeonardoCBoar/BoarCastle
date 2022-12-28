#include "wall.hpp"
#include "raylib.h"
#include <cstddef>
#include <cstdint>

Wall::Wall(const boar::IndexVector2 position)
    :position{position.x, 0, position.z}
{
}

void Wall::move_to(const boar::IndexVector2 target)
{
    this->position.x = target.x;
    this->position.y = SIZE.y/2;
    this->position.z = target.z;
}

void Wall::render() const
{
    if(!visible) return;
    DrawCube(this->position, this->SIZE.x, this->SIZE.y, this->SIZE.z, this->color);
    DrawCubeWires(this->position, this->SIZE.x, this->SIZE.y, this->SIZE.z, DARKGRAY);
}