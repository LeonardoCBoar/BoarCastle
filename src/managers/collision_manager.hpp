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
template <class GameObject> bool can_fit_object(const std::shared_ptr<GameObject> game_object) const
    {
        const int32_t half_size_x = game_object->SIZE.x / 2;
        const int32_t half_size_z = game_object->SIZE.z / 2;

        boar::IndexVector3 const index = game_object->position.to_index(1);
        for (int32_t x = index.x - half_size_x; x < index.x + half_size_x; x++)
        {
            for (int32_t z = index.z - half_size_z; z < index.z + half_size_z; z++)
            {
                const boar::IndexVector2 building_tile{x, z};
                if (!this->is_inside_borders(x, z) || !this->can_build_on_tile(building_tile))
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
                this->map[x][z].collision_state = MapTile::BUILDING;
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

    bool can_move_to_tile(const boar::IndexVector2 index) const
    {
        const MapTile::CollisionState collision = this->get_tile_collision(index);
        return collision != MapTile::CollisionState::BUILDING;
    }

    void move_to_tile(const boar::IndexVector2 origin, boar::IndexVector2 target)
    {
        MapTile& origin_tile = this->get_tile(origin);
        assert(origin_tile.collision_state != MapTile::BUILDING);
        assert(origin_tile.collision_state != MapTile::EMPTY);

        origin_tile.collision_state = MapTile::EMPTY;

        MapTile& target_tile = this->get_tile(target);
        assert(target_tile.collision_state == MapTile::EMPTY);
        target_tile.collision_state = MapTile::UNIT_MOVING_IN;
    }

    void unit_spawn_tile(const boar::IndexVector2 index)
    {
        MapTile& tile = this->get_tile(index);
        assert(tile.collision_state == MapTile::EMPTY);

        tile.collision_state = MapTile::UNIT_IDLE;
    }

    void unit_occupy_tile(const boar::IndexVector2 index)
    {
        MapTile& tile = this->get_tile(index);
        assert(tile.collision_state == MapTile::UNIT_MOVING_IN);

        tile.collision_state = MapTile::UNIT_IDLE;
    }        

    bool can_build_on_tile(const boar::IndexVector2 index) const
    {
        const MapTile::CollisionState collision = this->get_tile_collision(index);
        return collision == MapTile::CollisionState::EMPTY;
    }

    void building_occupy_tile(const boar::IndexVector2 index)
    {
        MapTile& tile = this->get_tile(index);
        assert(tile.collision_state == MapTile::EMPTY);
        tile.collision_state = MapTile::BUILDING;
    }        

    MapTile::CollisionState get_tile_collision(const boar::IndexVector2 index) const
    {
        return this->get_const_tile(index).collision_state;
    }


private:
    const MapTile& get_const_tile(const boar::IndexVector2 index) const
    {
        assert(index.x >= 0 && index.x < World::SIZE.x);
        assert(index.z >= 0 && index.z < World::SIZE.z);
        return this->map[index.x][index.z];
    }

    MapTile& get_tile(const boar::IndexVector2 index)
    {
        assert(index.x >= 0 && index.x < World::SIZE.x);
        assert(index.z >= 0 && index.z < World::SIZE.z);
        return this->map[index.x][index.z];
    }

};