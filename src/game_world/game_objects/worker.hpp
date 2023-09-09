#pragma once

// builtin
#include <cstdint>
#include <optional>
#include <vector>

// extern
#include <raylib.h>

// local
#include "../../utils/vector.hpp"



class Worker {

public:

    boar::IndexVector2 index{};
    Color color = RED;
    bool moving = false;
    boar::IndexVector2 step_target;
    boar::IndexVector2 target;
    std::vector<boar::IndexVector2> path;

private:

    constexpr static int32_t const SPEED = 30;
    boar::Vector3f render_pos{};
    float step_progress = 0;

public:

    Worker(boar::IndexVector2 const pos);

public:

    void move_to(boar::IndexVector2 const target);
    void update(float const delta);
    void render() const;

private:

    void update_movement(float const delta);
};