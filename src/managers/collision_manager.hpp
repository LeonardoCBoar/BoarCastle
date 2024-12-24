#pragma once

// builtin
#include <cassert>
#include <memory>

// local
#include "../game_world/world.hpp"

class CollisionManager
{
private:

    std::array<std::array<MapTile, World::SIZE.z>, World::SIZE.x>& map;

public:

    CollisionManager(std::array<std::array<MapTile, World::SIZE.z>, World::SIZE.x>& map): map{map} {}

    template <class GameObject>
    bool can_fit_object(const std::shared_ptr<GameObject> game_object) const
    {
        const int32_t half_size_x = game_object->SIZE.x / 2;
        const int32_t half_size_z = game_object->SIZE.z / 2;

        boar::IndexVector3 const index = game_object->position.to_index(1);
        for (int32_t x = index.x - half_size_x; x < index.x + half_size_x; x++)
        {
            for (int32_t z = index.z - half_size_z; z < index.z + half_size_z; z++)
            {
                if (!this->is_inside_borders(x, z) || !this->is_tile_empty(x, z))
                    return false;
            }
        }

        return true;
    }

    template <class GameObject>
    void add_object_collision(const std::shared_ptr<GameObject> game_object)
    {
        const size_t half_size_x = game_object->SIZE.x / 2;
        const size_t half_size_z = game_object->SIZE.z / 2;

        boar::IndexVector3 const index = game_object->position.to_index();
        for (size_t x = index.x - half_size_x; x < index.x + half_size_x; x++)
        {
            for (size_t z = index.z - half_size_z; z < index.z + half_size_z; z++)
            {
                this->map[x][z].empty = false;
            }
        }
    }

    template <class VectorT>
    bool is_inside_borders(const VectorT point) const
    {
        return this->is_inside_borders(static_cast<int32_t>(point.x), static_cast<int32_t>(point.z));
    }

    bool is_inside_borders(const int32_t x, const int32_t z) const
    {
        return x >= 0 && x < World::SIZE.x && z >= 0 && z < World::SIZE.z;
    }

    template <class VectorT>
    bool is_tile_empty(const VectorT point) const
    {
        return this->is_tile_empty(static_cast<int32_t>(point.x), static_cast<int32_t>(point.z));
    }

    bool is_tile_empty(const int32_t x, const int32_t z) const
    {
        assert(x >= 0 && x < World::SIZE.x);
        assert(z >= 0 && z < World::SIZE.z);
        return map[x][z].empty;
    }
};