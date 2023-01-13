#pragma once

#include <vector>
#include <optional>

#include <raylib.h>

#include "../../utils/vector.hpp"

class Worker
{
public:
    boar::IndexVector2 index{};
    boar::Vector3f render_pos{};

    Color color = RED;


    void move_to(const boar::IndexVector2 target);

    void update(const float delta);
    void render() const;
    

};