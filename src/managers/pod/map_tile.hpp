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
        UNIT_IDLE,
        BUILDING
    };

    CollisionState collision_state = CollisionState::EMPTY;
    boar::IndexVector2 index;

public:

    bool empty() const
    {
        return this->collision_state == CollisionState::EMPTY;
    }

    MapTile() = default;
    MapTile(boar::IndexVector2 const index): index{index} {}
};
