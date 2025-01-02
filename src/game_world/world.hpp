#pragma once

// builtin
#include <array>
#include <assert.h>
#include <cstdint>
#include <list>
#include <memory>
#include <queue>
#include <sys/types.h>

// local
#include "../managers/construction_manager.hpp"
#include "../managers/pathfinder.hpp"
#include "../managers/unit_manager.hpp"
#include "../utils/vector.hpp"
#include "../managers/pod/map_tile.hpp"



class Wall;
class HoverCamera;
class CollisionManager;
class Pathfinder;

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
    CollisionManager* collision_manager;
    std::unique_ptr<Pathfinder> pathfinder;

    std::vector<std::shared_ptr<Wall>> walls;

    // TODO: remove meaningless constexpr
    constexpr static boar::IndexVector2 SIZE{600, 600};

    InputMode current_input_mode;

private:

    std::array<std::array<MapTile, SIZE.z>, SIZE.x> map;

public:

    World();
    ~World();

public:

    MapTile* get_tile(boar::IndexVector2 const);
    void update();
    void render() const;


private:
};

inline World game_world;