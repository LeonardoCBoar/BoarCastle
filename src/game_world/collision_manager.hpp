#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include "../vector.hpp"

class CollisionManager
{
private:
    std::array<std::array<bool, 600>, 600> collision_matrix;

public:
    template<class GameObject>
    bool can_fit_object(const std::shared_ptr<GameObject> game_object) const
    {
        const size_t half_size_x = game_object->collision_matrix.size()/2;
        const size_t half_size_z = game_object->collision_matrix.at(0).size()/2;

        const boar::IndexVector3 index = game_object->position.to_index(1);
        for(size_t x = index.x - half_size_x; x < index.x + half_size_x; x++)
        {
            for(size_t z = index.z - half_size_z; z < index.z + half_size_z; z++)
            {
                if(!this->is_tile_empty(x,z))        
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

        const boar::IndexVector3 index = game_object->position.to_index();
        for(size_t x = index.x - half_size_x; x < index.x + half_size_x; x++)
        {
            for(size_t z = index.z - half_size_z; z < index.z + half_size_z; z++)
            {
                this->collision_matrix[x][z] = true;
            } 
        } 
    }

    template<class VectorT>
    bool is_inside_borders(const VectorT point) const
    {
        return this->is_inside_borders
        (
            static_cast<uint32_t>(point.x),
            static_cast<uint32_t>(point.z)
        );
    }
    
    bool is_inside_borders(const uint32_t x, const uint32_t z) const
    {
        return x >= 0 && x < this->collision_matrix.size() && 
               z >= 0 && z < this->collision_matrix.at(0).size();
    }

    template<class VectorT>
    bool is_tile_empty(const VectorT point) const
    {
        return this->is_tile_empty
        (
            static_cast<uint32_t>(point.x),
            static_cast<uint32_t>(point.z)
        );
    }
    
    bool is_tile_empty(const uint32_t x, const uint32_t z) const
    {
        if(!this->is_inside_borders(x,z))
            return false;

        return collision_matrix.at(x).at(z) == false;
    }
    
};

