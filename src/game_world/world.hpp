#pragma once

#include <array>
#include <vector>
#include <memory>

#include "../vector.hpp"
#include "collision_manager.hpp"

class Wall;
class CollisionManager;

class World
{

public:
    CollisionManager collision_manager{};

    World();

    void add_wall(std::shared_ptr<Wall> wall);
    void render() const;

private:
    std::vector<std::shared_ptr<Wall>> walls;

};

inline World game_world;