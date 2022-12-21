#pragma once

#include <array>
#include <memory>

#include "../vector.hpp"

class CollisionManager
{
private:
    std::array<std::array<bool, 2000>, 2000> collision_matrix;

public:
    template<class GameObject>
    bool can_fit_object(const std::shared_ptr<GameObject> game_object) const
    {
        const size_t half_size_x = game_object->collision_matrix.size()/2;
        const size_t half_size_z = game_object->collision_matrix.at(0).size()/2;

        const boar::IndexVector3 index = game_object->position.to_index(1);
        for(size_t x = index.x - half_size_x; x < index.x + half_size_x + 1; x++)
        {
            for(size_t z = index.z - half_size_z; z < index.z + half_size_z + 1; z++)
            {
                if(this->collision_matrix[x][z] == true)        
                    return false;
            } 
        } 

        return true;
    } 

    template<class GameObject>
    void add_object_collision(const std::shared_ptr<GameObject> game_object)
    {
        const size_t half_size_x = game_object->collision_matrix.size()/2;
        const size_t half_size_z = game_object->collision_matrix.at(0).size()/2;

        const boar::IndexVector3 index = game_object->position.to_index(1);
        for(size_t x = index.x - half_size_x; x < index.x + half_size_x + 1; x++)
        {
            for(size_t z = index.z - half_size_z; z < index.z + half_size_z + 1; z++)
            {
                this->collision_matrix[x][z] = true;
            } 
        } 
    }

private:
    bool is_inside_borders(const boar::IndexVector3 index) const
    {
        return index.x < 0 || index.x < this->collision_matrix.size() || 
            index.z < 0 || index.z > this->collision_matrix.at(0).size();
    }

    bool is_tile_empty(const boar::IndexVector3 index) const
    {
        if(!this->is_inside_borders(index))
            return false;

        return collision_matrix.at(index.x).at(index.z);
    }
};

