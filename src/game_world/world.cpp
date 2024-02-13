// header
#include "world.hpp"

// builtin
#include <list>

// extern
#include "raylib.h"

// local
#include "../utils/utils.hpp"
#include "game_objects/wall.hpp"


MapTile::MapTile() = default;
MapTile::MapTile(boar::IndexVector2 const index): index{index} {}


void MapTile::reset_pathfinding()
{
    this->parent = nullptr;
    this->movement_cost = 0;
    this->distance_cost = 0;
    this->total_cost = 0;
    this->pathfinding_started = false;
    this->visited = false;
}

void MapTile::setup_pathfinding(MapTile* parent, boar::IndexVector2 const target)
{
    this->parent = parent;
    if (this->parent != nullptr)
        this->movement_cost = parent->movement_cost + World::get_distance_cost(this->index - parent->index);
    else
        this->movement_cost = 0;

    this->distance_cost = this->index.manhattan_distance(target);
    this->total_cost = this->distance_cost + this->movement_cost;
    this->pathfinding_started = true;
}

Path World::construct_path(boar::IndexVector2 const start_index, const MapTile* const target_tile)
{
    std::vector<boar::IndexVector2> path{};

    const MapTile* current_tile = target_tile;
    while (current_tile->index != start_index)
    {
        path.push_back(current_tile->index);
        current_tile = current_tile->parent;
    }

    return path;
}

void World::reset_pathfinding()
{
    for (auto& row: this->map)
    {
        for (auto& tile: row)
        {
            tile.reset_pathfinding();
        }
    }
}

auto World::get_minimum_cost_tile(std::list<MapTile*>& open_list)
{
    auto current_tile_it = open_list.begin();
    int32_t current_tile_cost = (*current_tile_it)->total_cost;

    const auto list_end = open_list.end();
    for (auto tile = open_list.begin(); tile != list_end; tile++)
    {
        if ((*current_tile_it)->total_cost < current_tile_cost)
        {
            current_tile_it = tile;
        }
    }

    return current_tile_it;
}

World::World()
{
    TimeMeasurer world_start{"world"};


    for (int32_t x = 0; x < this->SIZE.x; x++)
    {
        for (int32_t z = 0; z < this->SIZE.z; z++)
        {
            this->map[x][z].index = boar::IndexVector2{x, z};
        }
    }

    world_start.print_time();

    this->construction_manager = std::make_unique<ConstructionManager>();
    this->unit_manager = std::make_unique<UnitManager>();
}

Path World::get_path(const boar::IndexVector2 origin, const boar::IndexVector2 target)
{
    this->reset_pathfinding();
    std::list<MapTile*> open{};

    // std::cout << origin << target;

    MapTile* origin_tile = this->get_tile(origin);
    // std::cout << origin_tile->set_id << "->" << target_tile->set_id << "\n";

    // if (origin_tile->set_id != target_tile->set_id)
    //     return Path{};


    origin_tile->setup_pathfinding(nullptr, target);
    open.push_back(origin_tile);

    while (!open.empty())
    {
        auto current_tile_it = World::get_minimum_cost_tile(open);
        auto current_tile = *current_tile_it;

        open.erase(current_tile_it);
        current_tile->visited = true;

        if (current_tile->index == target)
            return World::construct_path(origin, current_tile);

        for (int32_t x = -1; x < 2; x++)
        {
            for (int32_t z = -1; z < 2; z++)
            {
                const auto neighbor_index = boar::IndexVector2{current_tile->index.x + x, current_tile->index.z + z};

                if (neighbor_index == current_tile->index || !is_inside_borders(neighbor_index))
                    continue;

                MapTile& neighbor = this->map[neighbor_index.x][neighbor_index.z];
                  
                if (!neighbor.empty)
                    continue;
                
                
                if (!neighbor.pathfinding_started)
                {
                    neighbor.setup_pathfinding(current_tile, target);
                    open.push_back(&neighbor);
                }
                else
                {
                    const int32_t other_cost =
                        current_tile->movement_cost + World::get_distance_cost(neighbor_index - current_tile->index) + neighbor.index.manhattan_distance(target);
                    if (other_cost < neighbor.total_cost)
                        neighbor.setup_pathfinding(current_tile, target);
                }
            }
        }
    }

    return Path{};
}

int32_t World::get_distance_cost(boar::IndexVector2 const dir)
{
    const uint8_t non_zero_count = (dir.x != 0) + (dir.z != 0);
    assert(non_zero_count != 0); // this function shall not receive a null vector
    return LINEAR_DIST + (DIAGONAL_DIST - LINEAR_DIST) * (non_zero_count - 1);
}

MapTile* World::get_tile(boar::IndexVector2 const index)
{
    return &map[index.x][index.z];
}

void World::update()
{
    if (IsKeyDown(KEY_ONE))
        this->current_input_mode = InputMode::CONSTRUCTION;
    else if (IsKeyDown(KEY_TWO))
    {
        this->current_input_mode = InputMode::COMMAND;
    }
}

void World::render() const
{
    for (const auto& wall: this->walls)
    {
        wall->render();
    }
}