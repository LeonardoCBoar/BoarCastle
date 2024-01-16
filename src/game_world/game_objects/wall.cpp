// header
#include "wall.hpp"

// extern
#include "raylib.h"



Wall::Wall(boar::IndexVector2 const position): position{position.x, 0, position.z} {}

std::array<boar::IndexVector2, Wall::INTERACTION_SPOTS_COUNT> Wall::get_interaction_spots() const
{
    std::array<boar::IndexVector2, Wall::INTERACTION_SPOTS_COUNT> interaction_spots;
    size_t interaction_count = 0;

    const int32_t min_x = this->position.x - Wall::HALF_SIZE.x - 1;
    const int32_t min_z = this->position.z - Wall::HALF_SIZE.z - 1;
    const int32_t max_x = this->position.x + Wall::HALF_SIZE.x + 1;
    const int32_t max_z = this->position.z + Wall::HALF_SIZE.x + 1;

    for(int32_t x = min_x; x < max_x; x++)
    {
        for(int32_t z = min_z; z < max_z; z++)
        {
            const int32_t dist_x = x - this->position.x;
            const int32_t dist_z = z - this->position.z;

            if( ( dist_x == HALF_SIZE.x || dist_x == -HALF_SIZE.x - 1 ) ||
                ( dist_z == HALF_SIZE.z || dist_z == -HALF_SIZE.z - 1 ) )
            {
                interaction_spots[interaction_count++] = boar::IndexVector2{x, z}; 
            }
        }
    }

    return interaction_spots;
}

void Wall::move_to(boar::IndexVector2 const target)
{
    this->position.x = target.x;
    this->position.y = SIZE.y / 2;
    this->position.z = target.z;
}

void Wall::render() const
{
    //TODO: Worker not appearing behind transparent walls
    if (!visible)
        return;
    DrawCube(this->position, this->SIZE.x, this->SIZE.y, this->SIZE.z, this->color);
    DrawCubeWires(this->position, this->SIZE.x, this->SIZE.y, this->SIZE.z, DARKGRAY);
}