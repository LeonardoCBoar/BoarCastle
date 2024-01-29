#pragma once

// builtin
#include <vector>

// local
#include "../game_world/game_objects/worker.hpp"
#include "../utils/vector.hpp"
#include "pod/input_data.hpp"



class HoverCamera;

class UnitManager
{
public:

    std::vector<boar::IndexVector2> path{};
    std::vector<Worker> workers;

public:

    UnitManager();

public:

    void update(const float delta, const InputData& input_data);
    void render() const;
};