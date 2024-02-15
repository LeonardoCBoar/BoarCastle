#pragma once

// local
#include "../../utils/vector.hpp"




struct CollisionData {
    boar::IndexVector3 size;
    boar::IndexVector3 half_size;

    constexpr CollisionData(const boar::IndexVector3 size)
        : size{size}, half_size{size/2}
        {

        }
};

struct Transform {

    boar::IndexVector3 size;
    boar::IndexVector2 index;
    boar::IndexVector3 position;
};