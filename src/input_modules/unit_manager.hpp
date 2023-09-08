#pragma once

// builtin
#include <vector>

// local
#include "../utils/vector.hpp"
#include "../game_world/game_objects/worker.hpp"



class HoverCamera;

class UnitManager {

public:

    std::vector<boar::IndexVector2> path{};
    std::vector<Worker> workers;

private:

    HoverCamera const* const camera;

public:

    UnitManager(HoverCamera const* const camera);

public:

    void update(float const delta);
    void render() const;
};