#pragma once

#include <math.h>
#include <iostream>
#include <ostream>

#include <raylib.h>
static int conversions = 0;
namespace boar
{

template<class NumberT>
class CustomVector3 
{
public:
    NumberT x = 0;
    NumberT y = 0;
    NumberT z = 0;

    CustomVector3()
    {

    }

    CustomVector3(const Vector3 other)
        :x{other.x}, y{other.y}, z{other.z}
    {

    }

    CustomVector3(const NumberT x,const NumberT y, const NumberT z)
    : x{x},y{y},z{z}
    {

    }

    template<typename OtherNumberT>
    inline bool operator == (const CustomVector3<OtherNumberT>& other) const
    {
        return this->x == other.x && this->y == other.y && this->z == other.z;
    }

    inline CustomVector3<NumberT> operator + (const CustomVector3<NumberT>& other) const
    {
        return CustomVector3<NumberT>(this->x + other.x, this->y + other.y, this->z + other.z);
    }
    
    inline CustomVector3<NumberT>& operator += (const CustomVector3<NumberT>& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    inline CustomVector3<NumberT> operator - (const CustomVector3<NumberT>& other) const
    {
        return CustomVector3<NumberT>(this->x - other.x, this->y - other.y, this->z - other.z);
    }

    inline CustomVector3<NumberT> operator - () const
    {
        return CustomVector3<NumberT>(-this->x,-this->y,-this->z);
    }

    inline CustomVector3<NumberT> operator * (const CustomVector3<NumberT>& other) const
    {
        return CustomVector3<NumberT>(this->x * other.x, this->y * other.y, this->z * other.z);
    }

    template<typename OtherNumberT>
    inline CustomVector3<NumberT> operator * (const OtherNumberT& number) const
    {
        return CustomVector3<NumberT>(this->x * number, this->y * number, this->z * number);
    }

    inline CustomVector3<NumberT> operator / (const CustomVector3<NumberT>& other) const
    {
        return CustomVector3<NumberT>(this->x / other.x, this->y / other.y, this->z / other.z);
    }

    template<typename OtherNumberT>
    inline CustomVector3<NumberT> operator / (const OtherNumberT& number) const
    {
        return CustomVector3<NumberT>(this->x / number, this->y / number, this->z / number);
    }

    inline NumberT dot(const CustomVector3<NumberT>& other) const
    {
        return (this->x * other.x) + (this->y * other.y) + (this->z * other.z);
    }

    inline double cos_between(const CustomVector3<NumberT>& other) const
    {
        return this->dot(other)/(this->length() * other.length());
    }

    inline double angle_to(const CustomVector3<NumberT>& other) const
    {
        return acos(this->cos_between(other));
    }

    inline double distance_to(const CustomVector3<NumberT>& other) const
    {
        return (*this-other).length();
    }

    inline double length() const
    {
        return sqrt( (this->x*this->x) + (this->y*this->y) + (this->z*this->z) );
    }

    template<class IntegerNumberT>
    inline CustomVector3<IntegerNumberT> to_index(const IntegerNumberT step_size) const
    {
        return CustomVector3<IntegerNumberT>
        {
            static_cast<IntegerNumberT>(this->x) / step_size,
            static_cast<IntegerNumberT>(this->y) / step_size,
            static_cast<IntegerNumberT>(this->z) / step_size,
        };
    }

    inline CustomVector3<NumberT> normalized() const
    {
        const double length = this->length();
        return CustomVector3<NumberT>
        {
            static_cast<NumberT>(this->x / length),
            static_cast<NumberT>(this->y / length),
            static_cast<NumberT>(this->z / length),
        };
    }

    inline operator Vector3() const
    {
        return Vector3{this->x, this->y, this->z};
    }
};



using CustomVector3u16 = boar::CustomVector3<uint16_t>;
using CustomVector3u32 = boar::CustomVector3<uint32_t>;
using CustomVector3i16 = boar::CustomVector3<int16_t>;
using CustomVector3i32 = boar::CustomVector3<int16_t>;
using CustomVector3d   = boar::CustomVector3<double>;
using CustomVector3f   = boar::CustomVector3<float>;
using Vector3f         = CustomVector3f;

}

namespace Vectors
{
    inline boar::CustomVector3d ZERO{0,0,0};
    inline boar::CustomVector3d ONE {1,1,1};
}

template<class NumberT>
inline std::ostream& operator <<(std::ostream& ostream, const boar::CustomVector3<NumberT>& vector)
{
    return ostream << '(' << vector.x << ',' << vector.y << ',' << vector.z << ')';
}

template<class NumberT>
inline Vector3 operator + (const Vector3 vector, const boar::CustomVector3<NumberT>& other)
{
    return Vector3
    {
        vector.x + other.x,
        vector.y + other.y,
        vector.z + other.z
    };
}

inline Vector3 operator + (const Vector3 vector, Vector3 other)
{
    return Vector3
    {
        vector.x + other.x,
        vector.y + other.y,
        vector.z + other.z
    };
}

template<class NumberT>
inline Vector3 operator += (Vector3 vector, const boar::CustomVector3<NumberT>& other)
{
    vector.x += other.x;
    vector.y += other.y;
    vector.z += other.z;
    return vector;
}

template<class NumberT>
inline Vector3 operator * (const Vector3 vector, const NumberT multiplier)
{
    return Vector3
    {
        static_cast<float>(vector.x * multiplier),
        static_cast<float>(vector.y * multiplier),
        static_cast<float>(vector.z * multiplier)
    };
}

