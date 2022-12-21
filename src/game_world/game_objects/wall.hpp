#pragma once

#include "../../vector.hpp"
#include "raylib.h"
#include <array>

    
class Wall
{
public:
    constexpr static const boar::IndexVector3 SIZE{5,20,5};

    constexpr static const std::array<std::array<bool,SIZE.z>,SIZE.x> collision_matrix = 
    []{
        std::array<std::array<bool,SIZE.z>,SIZE.x> collision_matrix{};

        for(size_t x = 0; x < SIZE.x; x++)
        {
            for(size_t z = 0; z < SIZE.z; z++)
            {
                if(z%2 == 0)
                    collision_matrix[x][z] = false;
                else
                    collision_matrix[x][z] = true;
            }
        }

        return collision_matrix;
    }();

    Color color = GRAY;

    boar::Vector3d position;
    bool visible = true;

    Wall(const boar::Vector3d position = {0,0,0});
    void move_to(const boar::Vector3d target);

    void render() const;

};