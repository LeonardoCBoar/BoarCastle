#include "pathfinder.hpp"

#include <memory>

#include "collision_manager.hpp"
#include "../utils.hpp"

Pathfinder::Pathfinder(const std::shared_ptr<CollisionManager> collision_manager, const boar::IndexVector2 world_size)
    :collision_manager{collision_manager}, world_size{world_size}
{
    const int tile_count = world_size.x * world_size.z;
    this->open.reserve(tile_count);

    const auto measurer = TimeMeasurer{"Instantiated 4.000.000 tiles",TimeMeasurer::MILLISECOND};
    
    for(uint32_t x = 0; x < world_size.x; x++)
    {
        for(uint32_t z = 0; z < world_size.z; z++) 
        {
            this->open.emplace_back(std::make_shared<PathTile>(boar::IndexVector2{x,z}));
        }
    }

    measurer.print_time();

    this->closed.reserve(tile_count);

}

std::optional<std::shared_ptr<PathTile>> Pathfinder::get_path(const boar::IndexVector3 start, const boar::IndexVector3 target)
{

    return std::nullopt;
}