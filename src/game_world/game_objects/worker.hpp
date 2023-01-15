#pragma once

#include <cstdint>
#include <vector>
#include <optional>

#include <raylib.h>

#include "../../utils/vector.hpp"

class Worker
{
public:
    constexpr static const int32_t SPEED = 30;
    boar::IndexVector2 index{};
    boar::Vector3f render_pos{};

    Color color = RED;

    bool moving = false;
    float step_progress = 0;
    boar::IndexVector2 step_target;
    boar::IndexVector2 target;

    std::vector<boar::IndexVector2> path;

    void move_to(const boar::IndexVector2 target);

    void update(const float delta);
    void render() const;
    

};