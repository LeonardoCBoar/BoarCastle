#include "world.hpp"

#include <memory>

#include "collision_manager.hpp"
#include "pathfinder.hpp"
#include "game_objects/wall.hpp"

World::World()
{
    this->collision_manager = std::make_shared<CollisionManager>();
    this->pathfinder = std::make_shared<Pathfinder>(this->collision_manager, this->SIZE);

}

void World::add_wall(std::shared_ptr<Wall> wall)
{
    this->walls.push_back(wall);
    this->collision_manager->add_object_collision(wall);
}

void World::render() const
{
    for(const auto wall : this->walls)
    {
        wall->render();
    }
}