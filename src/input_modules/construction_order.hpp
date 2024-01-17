#pragma once

// builtin
#include <memory>



class Wall;

enum ConstructionOrderState
{
    NOT_ASSIGNED,
    IN_CONSTRUCTION,
    INACESSIBLE,
    FINISHED
};

struct ConstructionOrder
{
    float progress = 0;
    std::shared_ptr<Wall> construction;
    ConstructionOrderState state = NOT_ASSIGNED;
};
