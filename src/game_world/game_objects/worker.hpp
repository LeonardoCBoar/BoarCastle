#pragma once

// builtin
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

// local
#include "../../managers/pod/construction_order.hpp"
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

    uint64_t id;

    boar::IndexVector2 index{};
    boar::IndexVector2 step_target;
    boar::IndexVector2 target;
    std::vector<boar::IndexVector2> path;

    WorkerState current_state = IDLE;
    ConstructionOrder* target_construction = NULL;
    bool selected = false;

private:

    constexpr static const int32_t MOVE_SPEED = 50;
    constexpr static const float BUILD_SPEED = 4.0;
    boar::Vector3f render_pos{};
    float step_progress = 0;

public:

    Worker(boar::IndexVector2 const pos, const uint64_t id);

public:

    bool move_to(boar::IndexVector2 const target);
    void construct(ConstructionOrder* target_construction);
    void close_current_order(const ConstructionOrderState close_state);

    void update(const float delta);
    void render() const;

private:

    boar::IndexVector2 pop_next_movement();
    bool try_move_next_tile();
    void update_movement(const float delta);
};