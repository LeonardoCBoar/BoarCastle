#pragma once

// builtin
#include <array>
#include <assert.h>
#include <cstdint>
#include <list>
#include <memory>
#include <sys/types.h>
#include <queue>

// local
#include "../managers/construction_manager.hpp"
#include "../managers/unit_manager.hpp"
#include "../utils/vector.hpp"



class Wall;
class HoverCamera;

class MapTile
{
public:

    // Pathfinding
    MapTile* parent = nullptr;
    int32_t set_id = 0;
    int32_t movement_cost = 0;
    int32_t distance_cost = 0;
    int32_t total_cost = 0;

    bool pathfinding_started = false;
    bool visited = false;

    bool empty = true;
    boar::IndexVector2 index;

public:

    MapTile();
    MapTile(boar::IndexVector2 const index);

public:

    bool update_set();
    void reset_pathfinding();
    void setup_pathfinding(MapTile* parent, boar::IndexVector2 const target);
};

template <>
struct std::less<MapTile*>
{
    bool operator()(MapTile* const& a, MapTile* const& b)
    {
        return a->total_cost > b->total_cost;
    }
};


typedef std::vector<boar::IndexVector2> Path;

class World
{
public:

    enum InputMode
    {
        CONSTRUCTION,
        COMMAND
    };

public:

    std::unique_ptr<ConstructionManager> construction_manager;
    std::unique_ptr<UnitManager> unit_manager;
    std::vector<std::shared_ptr<Wall>> walls;

    // TODO: remove meaningless constexpr
    constexpr static boar::IndexVector2 SIZE{600, 600};
    constexpr static size_t DIR_COUNT = 8;
    constexpr static int32_t LINEAR_DIST = 10;
    constexpr static int32_t DIAGONAL_DIST = 14;

    InputMode current_input_mode;

private:

    std::array<std::array<MapTile, SIZE.z>, SIZE.x> map;

private:

    Path construct_path(boar::IndexVector2 const start_index, const MapTile* const target_tile);

public:

    World();

public:

    static int32_t get_distance_cost(boar::IndexVector2 const dir);
    Path get_path(boar::IndexVector2 const origin, boar::IndexVector2 const target);
    MapTile* get_tile(boar::IndexVector2 const);
    void update();
    void render() const;

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
        return x >= 0 && x < this->SIZE.x && z >= 0 && z < this->SIZE.z;
    }

    template <class VectorT>
    bool is_tile_empty(const VectorT point) const
    {
        return this->is_tile_empty(static_cast<int32_t>(point.x), static_cast<int32_t>(point.z));
    }

    bool is_tile_empty(const int32_t x, const int32_t z) const
    {
        assert(x >= 0 && x < SIZE.x);
        assert(z >= 0 && z < SIZE.z);
        return map[x][z].empty;
    }

private:

};

inline World game_world;