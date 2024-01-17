#pragma once

// builtin
#include <array>
#include <cstdint>
#include <list>
#include <memory>
#include <queue>
#include <sys/types.h>
#include <vector>

// local
#include "../utils/vector.hpp"
#include "../input_modules/construction_manager.hpp"
#include "../input_modules/unit_manager.hpp"



class Wall;
class HoverCamera;

class MapTile
{
public:

    // Pathfinding
    std::array<MapTile*, 8> neighbors{};
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
    void setup_pathfinding(boar::IndexVector2 const target, MapTile* parent, const int parent_dir_index);
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
    constexpr static std::array<int32_t, DIR_COUNT> MOVEMENT_COST{
        DIAGONAL_DIST, LINEAR_DIST, DIAGONAL_DIST, LINEAR_DIST, LINEAR_DIST, DIAGONAL_DIST, LINEAR_DIST, DIAGONAL_DIST};

    InputMode current_input_mode;
    bool queued_set_update = false;

private:

    std::array<std::array<MapTile, SIZE.z>, SIZE.x> map;

private:

    void set_tile_neighbors(MapTile* tile);
    Path construct_path(boar::IndexVector2 const start_index, const MapTile* const target_tile);
    void update_tile_sets();
    void reset_pathfinding();

public:

    World();
    void initialize_modules(const HoverCamera* const camera);

public:

    int32_t get_movement_cost(boar::IndexVector2 const) const;
    Path get_path(boar::IndexVector2 const origin, boar::IndexVector2 const target);
    MapTile* get_tile(boar::IndexVector2 const);
    void check_update_set();
    void update();
    void render() const;

    template <class GameObject>
    bool can_fit_object(const std::shared_ptr<GameObject> game_object) const
    {
        const size_t half_size_x = game_object->SIZE.x / 2;
        const size_t half_size_z = game_object->SIZE.z / 2;

        boar::IndexVector3 const index = game_object->position.to_index(1);
        for (size_t x = index.x - half_size_x; x < index.x + half_size_x; x++)
        {
            for (size_t z = index.z - half_size_z; z < index.z + half_size_z; z++)
            {
                if (!this->is_tile_empty(x, z))
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
        this->queued_set_update = true;
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
        return map[x][z].empty;
    }

private:

    // TODO: replace auto with explicit type
    static auto get_minimum_cost_tile(std::list<MapTile*>& tile_list);
};

inline World game_world;