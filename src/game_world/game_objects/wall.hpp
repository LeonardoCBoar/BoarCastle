#pragma once

#include "../../vector.hpp"
#include "raylib.h"
#include <array>
#include <utility>

    
class Wall
{
public:
    constexpr static const boar::IndexVector3 SIZE{4,20,4};

    constexpr static const std::array<std::array<bool,SIZE.z>,SIZE.x> collision_matrix = 
    []{
        std::array<std::array<bool,SIZE.z>,SIZE.x> collision_matrix{};

        for(size_t x = 0; x < SIZE.x; x++)
        {
            for(size_t z = 0; z < SIZE.z; z++)
            {
                collision_matrix[x][z] = true;
            }
        }

        return collision_matrix;
    }();

    Color color = GRAY;

    boar::Vector3u32 position;
    bool visible = true;

    Wall(const boar::Vector3u32 position = {0,0,0});
    void move_to(const boar::Vector3u32 target);

    void render() const;

};