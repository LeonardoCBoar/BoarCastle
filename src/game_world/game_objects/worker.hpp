#pragma once

#include <cstdint>
#include <vector>
#include <optional>

#include <raylib.h>

#include "../../utils/vector.hpp"

class Worker
{
public:
    boar::IndexVector2 index{};

    Color color = RED;

    bool moving = false;
    boar::IndexVector2 step_target;
    boar::IndexVector2 target;
    std::vector<boar::IndexVector2> path;


private:
    constexpr static const int32_t SPEED = 30;
    boar::Vector3f render_pos{};

    float step_progress = 0;


public:
    Worker(const boar::IndexVector2 pos);
    void move_to(const boar::IndexVector2 target);

    void update(const float delta);
    void render() const;

private:
    void update_movement(const float delta);
    

};