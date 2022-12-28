#include "world.hpp"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

#include "collision_manager.hpp"
#include "game_objects/wall.hpp"
#include "../utils.hpp"
#include "raylib.h"

MapTile::MapTile(const boar::IndexVector2 index)
    :index{index}
{

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

void MapTile::setup_pathfinding(const boar::IndexVector2 target, const std::shared_ptr<MapTile> parent, const int parent_dir_index)
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

World::World()
{
    this->collision_manager = std::make_shared<CollisionManager>();


    for(uint32_t x = 0; x < this->SIZE.x; x++)
    {
        for(uint32_t z = 0; z < this->SIZE.z; z++)
        {
            this->map[x][z] = std::make_shared<MapTile>(boar::IndexVector2{x,z});
        }
    }

    for(uint32_t x = 0; x < this->SIZE.x; x++)
    {
        for(uint32_t z = 0; z < this->SIZE.z; z++)
        {
           this->set_tile_neighbors(this->map[x][z]);
        }
    }

    //const auto path = this->get_path(boar::IndexVector2(1,1),boar::IndexVector2(500,3));
    //std::cout << path;
}

Path World::get_path(const boar::IndexVector2 origin, const boar::IndexVector2 target)
{
    this->reset_pathfinding();
    std::vector<std::shared_ptr<MapTile>> open{};

    //std::cout << origin << target;

    std::shared_ptr<MapTile> origin_tile = this->map[origin.x][origin.z];
    origin_tile->setup_pathfinding(target,nullptr,0);

    open.push_back(origin_tile);

    while(!open.empty())
    {
        std::shared_ptr<MapTile> current_tile = open[0];
        size_t current_tile_index = 0;

        for(size_t i = 1; i < open.size(); i++)
        {
            if(open[i]->total_cost < open[current_tile_index]->total_cost)
            {
                current_tile = open[i];
                current_tile_index = i;
            }
        }

        //std::cout << current_tile->index << std::endl;

        open.erase(open.begin() + current_tile_index);
        current_tile->visited = true;

        if(current_tile->index == target)
        {
            return World::construct_path(origin, current_tile);
        }

        for(size_t i = 0; i < DIR_COUNT; i++)
        {
            const auto neighbor = current_tile->neighbors[i];

            if(neighbor == nullptr || !collision_manager->is_tile_empty(neighbor->index)) continue;
            else if(!neighbor->pathfinding_started)
            {
                neighbor->setup_pathfinding(target, current_tile, 7-i);
                open.push_back(neighbor);
            }
            else
            {
                const double other_cost = current_tile->movement_cost + World::MOVEMENT_COST[i] + neighbor->index.manhattan_distance(target);
                if(other_cost < neighbor->total_cost)
                {
                    neighbor->setup_pathfinding(target, current_tile, 7-i);
                }
            }
        }
    }

    return Path{};
}

Path World::construct_path(const boar::IndexVector2 start_index, const std::shared_ptr<MapTile> target_tile)
{
    std::vector<boar::IndexVector2> path{};

    std::shared_ptr<MapTile> current_tile = target_tile;
    //std::cout << target_tile->index;
    while(current_tile->index != start_index)
    {
        path.push_back(current_tile->index);
        //std::cout << "looping at " << current_tile->index << std::endl;
        current_tile = current_tile->parent;
        //std::cout << "next will be at " << current_tile->index << std::endl;
    }

    return path;
}

void World::add_wall(std::shared_ptr<Wall> wall)
{
    this->walls.push_back(wall);
    this->map[wall->position.x][wall->position.z]->walkable = false;
    this->collision_manager->add_object_collision(wall);
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

void World::set_tile_neighbors(std::shared_ptr<MapTile> tile)
{
    int tile_count = 0;
    for(int x = -1; x < 2; x++)
    {
        for(int z = -1; z < 2; z++)
        {
            std::shared_ptr<MapTile> neighbor_tile;
            if(collision_manager->is_inside_borders(tile->index.x + x, tile->index.z + z))
            {
                const auto neighbor_index = boar::IndexVector2{tile->index.x + x, tile->index.z + z};
                if(neighbor_index == tile->index)
                    continue;

                neighbor_tile = this->map[neighbor_index.x][neighbor_index.z];
            }
            else
            {
                neighbor_tile = nullptr;
            }
            
            tile->neighbors[tile_count++] = neighbor_tile;

        }
    }
}

void World::reset_pathfinding()
{
    for(auto& row : this->map)
    {
        for(auto& tile : row)
        {
            tile->reset_pathfinding();
        }
    }
}