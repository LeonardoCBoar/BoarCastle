#pragma once

// builtin
#include <vector>

// local
#include "../game_world/game_objects/worker.hpp"
#include "../utils/vector.hpp"



class HoverCamera;

class UnitManager
{
public:

    std::vector<boar::IndexVector2> path{};
    std::vector<Worker> workers;

private:

    const HoverCamera* const camera;

public:

    UnitManager(const HoverCamera* const camera);

public:

    void update(const float delta);
    void render() const;
};