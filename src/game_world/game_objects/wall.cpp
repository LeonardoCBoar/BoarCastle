// header
#include "wall.hpp"

// extern
#include "raylib.h"



Wall::Wall(boar::IndexVector2 const position): position{position.x, 0, position.z} {}

void Wall::move_to(boar::IndexVector2 const target)
{
    this->position.x = target.x;
    this->position.y = SIZE.y / 2;
    this->position.z = target.z;
}

void Wall::render() const
{
    if (!visible)
        return;
    DrawCube(this->position, this->SIZE.x, this->SIZE.y, this->SIZE.z, this->color);
    DrawCubeWires(this->position, this->SIZE.x, this->SIZE.y, this->SIZE.z, DARKGRAY);
}