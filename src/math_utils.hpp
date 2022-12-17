#pragma once

#include <iostream>

#include "raylib.h"

#include "vector.hpp"


template<class NumberT>
Vector3 operator + (const Vector3 vector, const boar::Vector3<NumberT>& other)
{
    return Vector3
    {
        vector.x + other.x,
        vector.y + other.y,
        vector.z + other.z
    };
}

template<class NumberT>
Vector3 operator += (Vector3 vector, const boar::Vector3<NumberT>& other)
{
    vector.x += other.x;
    vector.y += other.y;
    vector.z += other.z;
    return vector;
}

template<class NumberT>
boar::Vector3<NumberT>::operator Vector3() const
{
    return Vector3{this->x, this->y, this->z};
}