#pragma once

// local
#include "../../utils/vector.hpp"

class MapTile
{
public:
    enum CollisionState
    {
        EMPTY,
        UNIT_MOVING_IN,
        UNIT_MOVING_OUT,
        UNIT_MOVING_IN_OUT,
        OCCUPIED,
    };

    bool empty = true;
    CollisionState collision_state = CollisionState::EMPTY;
    boar::IndexVector2 index;

public:

    MapTile() = default;
    MapTile(boar::IndexVector2 const index): index{index} {}
};
