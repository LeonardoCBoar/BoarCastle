#include "world.hpp"
#include <memory>

#include "game_objects/wall.hpp"

World::World()
{
}

void World::add_wall(std::shared_ptr<Wall> wall)
{
    this->walls.push_back(wall);
    this->collision_manager.add_object_collision(wall);
}

void World::render() const
{
    for(const auto wall : this->walls)
    {
        wall->render();
    }
}