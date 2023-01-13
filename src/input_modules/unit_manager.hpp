#pragma once

#include "../utils/vector.hpp"
#include <vector>

#include "../game_world/game_objects/worker.hpp"

class HoverCamera;

class UnitMananger
{
public:
    UnitMananger(const HoverCamera* const camera);

    boar::IndexVector2 start_point{};
    boar::IndexVector2 target_point{};

    std::vector<boar::IndexVector2> path{};

    std::vector<Worker> workers;

    void update(const float delta);
    void render() const;

private:
    const HoverCamera* const camera;
};