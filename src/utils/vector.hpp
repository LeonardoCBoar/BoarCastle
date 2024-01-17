#pragma once

// builtin
#include <cmath>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <sys/types.h>

// extern
#include <raylib.h>


// TODO: tornar métodos constantes em [[nodiscard]]
// TODO: remover "inlines" desnecessários
// TODO: substituir métodos genéricos por funções de conversão

namespace boar
{

    template <class NumberT>
    class CustomVector2
    {
    public:

        NumberT x;
        NumberT z;

    public:

        constexpr CustomVector2(const NumberT x = 0, const NumberT z = 0): x{x}, z{z} {}

    public:

        inline CustomVector2<NumberT> operator+(const CustomVector2<NumberT> other) const
        {
            return CustomVector2<NumberT>{this->x + other.x, this->z + other.z};
        }

        inline CustomVector2<NumberT> operator-(const CustomVector2<NumberT> other) const
        {
            return CustomVector2<NumberT>{this->x - other.x, this->z - other.z};
        }

        inline CustomVector2<NumberT> operator*(const CustomVector2<NumberT> other) const
        {
            return CustomVector2<NumberT>{this->x * other.x, this->z * other.z};
        }

        inline CustomVector2<NumberT> operator/(const CustomVector2<NumberT> other) const
        {
            return CustomVector2<NumberT>{this->x / other.x, this->z / other.z};
        }

        inline bool operator==(const CustomVector2<NumberT> other) const
        {
            return this->x == other.x && this->z == other.z;
        }

        inline bool operator!=(const CustomVector2<NumberT> other) const
        {
            return this->x != other.x || this->z != other.z;
        }

        inline int32_t manhattan_distance(const CustomVector2<NumberT> other) const
        {
            return abs(static_cast<int16_t>(this->x) - static_cast<int16_t>(other.x)) +
                   abs(static_cast<int16_t>(this->z) - static_cast<int16_t>(other.z));
        }
    };

    template <class NumberT>
    inline std::ostream& operator<<(std::ostream& ostream, const boar::CustomVector2<NumberT>& vector)
    {
        return ostream << '(' << vector.x << ',' << vector.z << ")\n";
    }

    using IndexVector2 = CustomVector2<int32_t>;

    template <class NumberT>
    class CustomVector3
    {
    public:

        NumberT x = 0;
        NumberT y = 0;
        NumberT z = 0;

    public:

        constexpr CustomVector3() {}
        constexpr CustomVector3(const Vector3 other): x{other.x}, y{other.y}, z{other.z} {}
        constexpr CustomVector3(const NumberT x, const NumberT y, const NumberT z): x{x}, y{y}, z{z} {}

    public:

        template <typename OtherNumberT>
        inline bool operator==(const CustomVector3<OtherNumberT>& other) const
        {
            return this->x == other.x && this->y == other.y && this->z == other.z;
        }

        inline CustomVector3<NumberT> operator+(const CustomVector3<NumberT>& other) const
        {
            return CustomVector3<NumberT>(this->x + other.x, this->y + other.y, this->z + other.z);
        }

        inline CustomVector3<NumberT>& operator+=(const CustomVector3<NumberT>& other)
        {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
            return *this;
        }

        inline CustomVector3<NumberT> operator-(const CustomVector3<NumberT>& other) const
        {
            return CustomVector3<NumberT>(this->x - other.x, this->y - other.y, this->z - other.z);
        }

        inline CustomVector3<NumberT> operator-() const
        {
            return CustomVector3<NumberT>(-this->x, -this->y, -this->z);
        }

        inline CustomVector3<NumberT> operator*(const CustomVector3<NumberT>& other) const
        {
            return CustomVector3<NumberT>(this->x * other.x, this->y * other.y, this->z * other.z);
        }

        template <typename OtherNumberT>
        inline CustomVector3<NumberT> operator*(const OtherNumberT& number) const
        {
            return CustomVector3<NumberT>(this->x * number, this->y * number, this->z * number);
        }

        inline CustomVector3<NumberT> operator/(const CustomVector3<NumberT>& other) const
        {
            return CustomVector3<NumberT>(this->x / other.x, this->y / other.y, this->z / other.z);
        }

        template <typename OtherNumberT>
        constexpr inline CustomVector3<NumberT> operator/(const OtherNumberT& number) const
        {
            return CustomVector3<NumberT>(this->x / number, this->y / number, this->z / number);
        }

        inline double distance_to(const CustomVector3<NumberT>& other) const
        {
            return (*this - other).length();
        }

        inline double length() const
        {
            return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
        }

        inline CustomVector3<int32_t> to_index(const int32_t step_size = 1) const
        {
            return CustomVector3<int32_t>{
                static_cast<int32_t>(this->x) / step_size,
                0,
                static_cast<int32_t>(this->z) / step_size,
            };
        }

        inline CustomVector2<int32_t> to_index2(const int32_t step_size = 1) const
        {
            return CustomVector2<int32_t>{
                static_cast<int32_t>(this->x) / step_size,
                static_cast<int32_t>(this->z) / step_size,
            };
        }

        inline CustomVector3<double> to_global_center(const double step_size) const
        {
            return CustomVector3<double>{
                static_cast<double>(this->x) * step_size + step_size / 2,
                static_cast<double>(this->y) * step_size,
                static_cast<double>(this->z) * step_size + step_size / 2,
            };
        }

        inline CustomVector3<NumberT> normalized() const
        {
            const double length = this->length();
            return CustomVector3<NumberT>{
                static_cast<NumberT>(this->x / length),
                static_cast<NumberT>(this->y / length),
                static_cast<NumberT>(this->z / length),
            };
        }

        inline operator Vector3() const
        {
            return Vector3{
                static_cast<float>(this->x),
                static_cast<float>(this->y),
                static_cast<float>(this->z),
            };
        }

        inline CustomVector2<int32_t> index_vector() const
        {
            return IndexVector2{this->x, this->z};
        }
    };

    using Vector3u16 = boar::CustomVector3<uint16_t>;
    using Vector3u32 = boar::CustomVector3<int32_t>;
    using Vector3i16 = boar::CustomVector3<int16_t>;
    using Vector3i32 = boar::CustomVector3<int32_t>;
    using Vector3d = boar::CustomVector3<double>;
    using Vector3f = boar::CustomVector3<float>;
    using IndexVector3 = Vector3i32;

} // namespace boar

namespace Vectors
{
    inline boar::Vector3u32 ZERO{0, 0, 0};
    inline boar::Vector3u32 ONE{1, 1, 1};
} // namespace Vectors

template <class NumberT>
inline std::ostream& operator<<(std::ostream& ostream, const boar::CustomVector3<NumberT>& vector)
{
    return ostream << '(' << vector.x << ',' << vector.y << ',' << vector.z << ")\n";
}

template <class NumberT>
inline Vector3 operator+(const Vector3 vector, const boar::CustomVector3<NumberT>& other)
{
    return Vector3{static_cast<float>(vector.x + other.x), static_cast<float>(vector.y + other.y),
                   static_cast<float>(vector.z + other.z)};
}

inline Vector3 operator+(const Vector3 vector, Vector3 other)
{
    return Vector3{static_cast<float>(vector.x + other.x), static_cast<float>(vector.y + other.y),
                   static_cast<float>(vector.z + other.z)};
}

template <class NumberT>
inline Vector3 operator+=(Vector3 vector, const boar::CustomVector3<NumberT>& other)
{
    vector.x += other.x;
    vector.y += other.y;
    vector.z += other.z;
    return static_cast<Vector3>(vector);
}

template <class NumberT>
inline Vector3 operator*(const Vector3 vector, const NumberT multiplier)
{
    return Vector3{static_cast<float>(vector.x * multiplier), static_cast<float>(vector.y * multiplier),
                   static_cast<float>(vector.z * multiplier)};
}
