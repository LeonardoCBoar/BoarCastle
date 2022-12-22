#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <math.h>
#include <vector>

#include "../vector.hpp"

class CollisionManager;

class PathTile
{
public:
    boar::IndexVector2 position{0,0};
    std::shared_ptr<PathTile> previous_tile = nullptr;
    double movement_cost = 0;
    double distance_cost = 0;
    double total_cost = 0;

    PathTile(const boar::IndexVector2 position)
        :position{position}
    {

    };

};


class Pathfinder
{
public:
    Pathfinder(const std::shared_ptr<CollisionManager> collision_manger, const boar::IndexVector2 world_size);

    std::optional<std::shared_ptr<PathTile>> get_path(const boar::IndexVector3 start, const boar::IndexVector3 target);

private:
    const double DIAGONAL_DISTANCE = 10 * sqrt(2);
    const double LINEAR_DISTANCE   = 10;
    const boar::IndexVector2 world_size;

    const std::shared_ptr<CollisionManager> collision_manager;

    std::vector<std::shared_ptr<PathTile>> open{};
    std::vector<std::shared_ptr<PathTile>> closed{};

};