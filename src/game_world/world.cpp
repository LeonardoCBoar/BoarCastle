// header
#include "world.hpp"

// builtin
#include <list>
#include <queue>
#include <vector>


// extern
#include "raylib.h"

// local
#include "../managers/collision_manager.hpp"
#include "../managers/pathfinder.hpp"
#include "../utils/utils.hpp"
#include "game_objects/wall.hpp"


MapTile::MapTile() = default;
MapTile::MapTile(boar::IndexVector2 const index): index{index} {}


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
    this->collision_manager = new CollisionManager(this->map);
    this->pathfinder = std::make_unique<Pathfinder>(World::SIZE);
    this->unit_manager = std::make_unique<UnitManager>();
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

World::~World()
{
    delete this->collision_manager;
}