#pragma once

#include <array>
#include <vector>
#include <memory>

#include "../vector.hpp"

class Wall;
class CollisionManager;
class Pathfinder;

class World
{

public:
    constexpr static const boar::IndexVector2 SIZE{600,600};
    std::shared_ptr<CollisionManager> collision_manager;
    std::shared_ptr<Pathfinder> pathfinder;

    World();

    void add_wall(std::shared_ptr<Wall> wall);
    void render() const;

private:
    std::vector<std::shared_ptr<Wall>> walls;

};

inline World game_world;