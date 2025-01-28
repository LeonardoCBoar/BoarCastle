// header
#include "pathfinder.hpp"

// local
#include "../game_world/world.hpp"
#include "collision_manager.hpp"
#include "pod/map_tile.hpp"



/* -------------------------------------------------------------------------- */
/*                               Pathfinder tile                              */
/* -------------------------------------------------------------------------- */

void PathfindingTile::reset_pathfinding()
{
    this->parent = nullptr;
    this->movement_cost = 0;
    this->distance_cost = 0;
    this->total_cost = 0;
    this->pathfinding_started = false;
    this->visited = false;
}

void PathfindingTile::setup_pathfinding(PathfindingTile* parent, boar::IndexVector2 const target)
{
    this->parent = parent;
    if (this->parent != nullptr)
        this->movement_cost =
            parent->movement_cost + Pathfinder::get_distance_cost(this->map_tile->index - parent->map_tile->index);
    else
        this->movement_cost = 0;

    this->distance_cost = this->map_tile->index.squared_euclidian_distance(target);
    this->total_cost = this->distance_cost + this->movement_cost;
    this->pathfinding_started = true;
}



/* -------------------------------------------------------------------------- */
/*                                 Pathfinder                                 */
/* -------------------------------------------------------------------------- */

Pathfinder::Pathfinder(const boar::IndexVector2 map_size): map_size(map_size)
{
    this->map.resize(map_size.x);
    for (int32_t x = 0; x < map_size.x; x++)
    {
        this->map[x].resize(map_size.z);
        for (int32_t z = 0; z < map_size.z; z++)
        {
            this->map[x][z].map_tile = game_world.get_tile({x, z});
        }
    }
}


Path Pathfinder::construct_path(const boar::IndexVector2 start_index, const PathfindingTile* const target_tile)
{
    std::vector<boar::IndexVector2> path{};

    const PathfindingTile* current_tile = target_tile;
    while (current_tile->map_tile->index != start_index)
    {
        path.push_back(current_tile->map_tile->index);
        current_tile = current_tile->parent;
    }

    return path;
}


int32_t Pathfinder::get_distance_cost(boar::IndexVector2 const dir)
{
    const uint8_t non_zero_count = (dir.x != 0) + (dir.z != 0);
    assert(non_zero_count != 0); // this function shall not receive a null vector
    return LINEAR_DIST + (DIAGONAL_DIST - LINEAR_DIST) * (non_zero_count - 1);
}


Path Pathfinder::get_path(const boar::IndexVector2 origin, const boar::IndexVector2 target)
{
    assert(game_world.collision_manager->is_inside_borders(target));
    const PathfindingTile* const target_tile = &this->map[target.x][target.z];
    if (!target_tile->map_tile->empty())
        return {};

    std::priority_queue<PathfindingTile*> open{};
    std::vector<PathfindingTile*> used_tiles;

    auto reset_used_tiles = [&used_tiles]()
    {
        for (auto tile: used_tiles)
            tile->reset_pathfinding();
    };

    auto push_tile = [&open, &used_tiles](PathfindingTile* tile)
    {
        open.push(tile);
        used_tiles.push_back(tile);
    };

    PathfindingTile* origin_tile = &this->map[origin.x][origin.z];

    origin_tile->setup_pathfinding(nullptr, target);
    push_tile(origin_tile);

    while (!open.empty())
    {
        auto current_tile = open.top();
        open.pop();

        current_tile->visited = true;

        if (current_tile->map_tile->index == target)
        {
            auto path = Pathfinder::construct_path(origin, current_tile);
            reset_used_tiles();
            return path;
        }

        for (int32_t x = -1; x < 2; x++)
        {
            for (int32_t z = -1; z < 2; z++)
            {
                const boar::IndexVector2 tile_index = current_tile->map_tile->index;
                const boar::IndexVector2 neighbor_index = tile_index + boar::IndexVector2{x, z};

                if (neighbor_index == tile_index ||
                    !game_world.collision_manager->is_inside_borders(neighbor_index))
                    continue;

                PathfindingTile& neighbor = this->map[neighbor_index.x][neighbor_index.z];
                MapTile::CollisionState neighbor_collision = neighbor.map_tile->collision_state; 

                if (!neighbor.map_tile->empty())
                    continue;
                
                if(x != 0 && z != 0)
                {
                    if(!this->map[tile_index.x + x][tile_index.z].map_tile->empty())
                        continue;
                    if(!this->map[tile_index.x][tile_index.z + z].map_tile->empty())
                        continue;
                }

                if (!neighbor.pathfinding_started)
                {
                    neighbor.setup_pathfinding(current_tile, target);
                    push_tile(&neighbor);
                }
                else
                {
                    const int32_t other_cost =
                        current_tile->movement_cost +
                        Pathfinder::get_distance_cost(neighbor_index - tile_index) +
                        neighbor.map_tile->index.squared_euclidian_distance(target);
                    if (other_cost < neighbor.total_cost)
                        neighbor.setup_pathfinding(current_tile, target);
                }
            }
        }
    }

    reset_used_tiles();
    return Path{};
}
