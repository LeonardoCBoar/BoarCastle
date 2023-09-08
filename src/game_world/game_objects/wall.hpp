#pragma once

// builtin
#include <array>
#include <utility>

// extern
#include "raylib.h"

// local
#include "../../utils/vector.hpp"



class Wall {

public:

    constexpr static boar::IndexVector3 const SIZE{2, 6, 2};

    constexpr static std::array<std::array<bool, SIZE.z>, SIZE.x> const collision_matrix = [] {
        std::array<std::array<bool, SIZE.z>, SIZE.x> collision_matrix{};

        for (size_t x = 0; x < SIZE.x; x++) {
            for (size_t z = 0; z < SIZE.z; z++) {
                collision_matrix[x][z] = true;
            }
        }

        return collision_matrix;
    }();

    Color color = GRAY;

    boar::IndexVector3 position;
    bool visible = true;

public:

    Wall(boar::IndexVector2 const position = {0, 0});

public:

    void move_to(boar::IndexVector2 const target);
    void render() const;
};