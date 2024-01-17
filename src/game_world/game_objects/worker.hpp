#pragma once

// builtin
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

// extern
#include <raylib.h>

// local
#include "../../input_modules/construction_order.hpp"
#include "../../utils/vector.hpp"



class Wall;

class Worker
{
    enum WorkerState
    {
        IDLE,
        MOVING,
        BUILDING,
    };

public:

    Color color = RED;

    boar::IndexVector2 index{};
    boar::IndexVector2 step_target;
    boar::IndexVector2 target;
    std::vector<boar::IndexVector2> path;

    WorkerState current_state = IDLE;
    ConstructionOrder* target_construction = NULL;

private:

    constexpr static const int32_t MOVE_SPEED = 30;
    constexpr static const float BUILD_SPEED = 2.0;
    boar::Vector3f render_pos{};
    float step_progress = 0;

public:

    Worker(boar::IndexVector2 const pos);

public:

    bool move_to(boar::IndexVector2 const target);
    void construct(ConstructionOrder* target_construction);
    void close_current_order(const ConstructionOrderState close_state);

    void update(const float delta);
    void render() const;

private:

    void update_movement(const float delta);
};