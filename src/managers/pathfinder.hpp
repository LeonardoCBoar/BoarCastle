#pragma once

// builtin
#include <cstdint>
#include <queue>

// local
#include "../utils/vector.hpp"



class MapTile;

class PathfindingTile
{
public:
    
    PathfindingTile* parent = nullptr;
    int32_t set_id = 0;
    int32_t movement_cost = 0;
    int32_t distance_cost = 0;
    int32_t total_cost = 0;

    bool pathfinding_started = false;
    bool visited = false;

    // TODO: in the future, we will check if a tile is occupied by calling to the
    // collision manager, but for now we check directly in the tile
    MapTile* map_tile;

public:
    
    void reset_pathfinding();
    void setup_pathfinding(PathfindingTile* parent, boar::IndexVector2 const target);
};

// impl lower-than operator
template <>
struct std::less<PathfindingTile*>
{
    bool operator()(PathfindingTile* const& a, PathfindingTile* const& b)
    {
        return a->total_cost > b->total_cost;
    }
};


using Path = std::vector<boar::IndexVector2>;

class Pathfinder
{
private:
    constexpr static int32_t LINEAR_DIST = 10;
    constexpr static int32_t DIAGONAL_DIST = 14;
    
    const boar::IndexVector2 map_size;
    
    // TODO: Maybe it's not necessary to have one Pathfinding tile for each MapTile
    std::vector<std::vector<PathfindingTile>> map;

public:
    
    Pathfinder(const boar::IndexVector2 map_size);
    Path get_path(const boar::IndexVector2 origin, const boar::IndexVector2 target);
    static int32_t get_distance_cost(const boar::IndexVector2 dir);

private:

    static Path construct_path(const boar::IndexVector2 start_index, const PathfindingTile* const target_tile);
};

