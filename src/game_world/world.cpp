#include "world.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <list>

#include "game_objects/wall.hpp"
#include "../utils/utils.hpp"
#include "raylib.h"

MapTile::MapTile()
{

}

MapTile::MapTile(const boar::IndexVector2 index)
    :index{index}
{

}

bool MapTile::update_set()
{
    bool updated = false;
    for(const MapTile* const neighbor : this->neighbors)
    {
        if(neighbor != nullptr && neighbor->empty && neighbor->set_id < this->set_id)
        {
            this->set_id = neighbor->set_id;
            updated = true;
        }
    }
    return updated;
}

void MapTile::reset_pathfinding()
{
    this->parent = nullptr;
    this->movement_cost = 0;
    this->distance_cost = 0;
    this->total_cost = 0;
    this->pathfinding_started = false;
    this->visited = false;
}

void MapTile::setup_pathfinding(const boar::IndexVector2 target, MapTile* parent, const int parent_dir_index)
{
    this->parent = parent;
    if(this->parent != nullptr)
        this->movement_cost = parent->movement_cost + World::MOVEMENT_COST[parent_dir_index];
    else
        this->movement_cost = 0;

    this->distance_cost = this->index.manhattan_distance(target);
    this->total_cost = this->distance_cost + this->movement_cost;
    this->pathfinding_started = true;
}


void World::set_tile_neighbors(MapTile* tile)
{
    int tile_count = 0;
    for(int x = -1; x < 2; x++)
    {
        for(int z = -1; z < 2; z++)
        {
            MapTile* neighbor_tile;
            if(is_inside_borders(tile->index.x + x, tile->index.z + z))
            {
                const auto neighbor_index = boar::IndexVector2{tile->index.x + x, tile->index.z + z};
                if(neighbor_index == tile->index)
                    continue;

                neighbor_tile = &this->map[neighbor_index.x][neighbor_index.z];
            }
            else
            {
                neighbor_tile = nullptr;
            }
            
            tile->neighbors[tile_count++] = neighbor_tile;

        }
    }
}

Path World::construct_path(const boar::IndexVector2 start_index, const MapTile* const target_tile)
{
    std::vector<boar::IndexVector2> path{};

    const MapTile* current_tile = target_tile;
    while(current_tile->index != start_index)
    {
        path.push_back(current_tile->index);
        current_tile = current_tile->parent;
    }

    return path;
}

void World::update_tile_sets()
{
    TimeMeasurer timer{"Sets updated in"};
    int32_t set_id = 0;
    for(auto& row : this->map)
    {
        for(auto& tile : row)
        {
            tile.set_id = set_id++;
        }
    }

    bool set_updated = true;
    while(set_updated)
    {
        set_updated = false;
        for(auto& row : this->map)
        {
            for(auto& tile : row)
            {
                if(tile.update_set())
                    set_updated = true;
            }
        }
    }

    timer.print_time();
}

void World::reset_pathfinding()
{
    for(auto& row : this->map)
    {
        for(auto& tile : row)
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
    for(auto tile = open_list.begin(); tile != list_end; tile++)
    {
        if( (*current_tile_it)->total_cost < current_tile_cost)
        {
            current_tile_it = tile;
        }
    }

    return current_tile_it;
}

World::World()
{
    TimeMeasurer world_start{"world"};


    for(int32_t x = 0; x < this->SIZE.x; x++)
    {
        for(int32_t z = 0; z < this->SIZE.z; z++)
        {
            this->map[x][z].index  = boar::IndexVector2{x,z};
        }
    }

    for(int32_t x = 0; x < this->SIZE.x; x++)
    {
        for(int32_t z = 0; z < this->SIZE.z; z++)
        {
           this->set_tile_neighbors(&this->map[x][z]);
        }
    }
    world_start.print_time();

    this->update_tile_sets();
    TimeMeasurer a{"aaaaaa"};
    const auto path = this->get_path(boar::IndexVector2(1,1),boar::IndexVector2(200,3));
    a.print_time();
    //std::cout << path;
}

Path World::get_path(const boar::IndexVector2 origin, const boar::IndexVector2 target)
{
    this->reset_pathfinding();
    std::list<MapTile*> open{};

    //std::cout << origin << target;

    MapTile* origin_tile = this->get_tile(origin);
    MapTile* target_tile = this->get_tile(target);
    //std::cout << origin_tile->set_id << "->" << target_tile->set_id << "\n";

    if(origin_tile->set_id != target_tile->set_id)
    {
        std::cout << "ret here";
        return Path{};
    }


    origin_tile->setup_pathfinding(target,nullptr,0);
    open.push_back(origin_tile);

    while(!open.empty())
    {
        
        auto current_tile_it = World::get_minimum_cost_tile(open);
        auto current_tile = *current_tile_it;

        open.erase(current_tile_it);
        current_tile->visited = true;

        if(current_tile->index == target)
        {
            return World::construct_path(origin, current_tile);
        }

        for(size_t i = 0; i < DIR_COUNT; i++)
        {
            const auto neighbor = current_tile->neighbors[i];

            if(neighbor == nullptr || !neighbor->empty) continue;
            else if(!neighbor->pathfinding_started)
            {
                neighbor->setup_pathfinding(target, current_tile, 7-i);
                open.push_back(neighbor);
            }
            else
            {
                const int32_t other_cost = current_tile->movement_cost + World::MOVEMENT_COST[i] + neighbor->index.manhattan_distance(target);
                if(other_cost < neighbor->total_cost)
                {
                    neighbor->setup_pathfinding(target, current_tile, 7-i);
                }
            }
        }
    }

    return Path{};
}

MapTile* World::get_tile(const boar::IndexVector2 index)
{
    return &map[index.x][index.z];
}

void World::add_wall(std::shared_ptr<Wall> wall)
{
    this->walls.push_back(wall);
    this->add_object_collision(wall);
    this->update_tile_sets();
}

void World::update()
{
    if(IsKeyDown(KEY_ONE)) this->current_input_mode = InputMode::CONSTRUCTION;
    else if(IsKeyDown(KEY_TWO)) this->current_input_mode = InputMode::COMMAND;
}

void World::render() const
{
    for(const auto wall : this->walls)
    {
        wall->render();
    }
}
