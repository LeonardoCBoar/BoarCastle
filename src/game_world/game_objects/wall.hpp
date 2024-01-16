#pragma once

// builtin
#include <array>
#include <utility>

// extern
#include "raylib.h"

// local
#include "../../utils/vector.hpp"

const Color WALL_COLOR = GRAY;

class Wall {

public:

    constexpr static boar::IndexVector3 const SIZE{2, 6, 2};
    constexpr static boar::IndexVector3 const HALF_SIZE = SIZE/2;
    constexpr static uint32_t INTERACTION_SPOTS_COUNT = SIZE.x * 2 + SIZE.z * 2 + 4;

    Color color = GRAY;

    boar::IndexVector3 position;
    bool visible = true;

public:

    Wall(boar::IndexVector2 const position = {0, 0});

public:

    bool is_in_interaction_range(const boar::IndexVector2 pos) const;

    std::array<boar::IndexVector2, INTERACTION_SPOTS_COUNT> get_interaction_spots() const;

    void move_to(boar::IndexVector2 const target);
    void render() const;
};