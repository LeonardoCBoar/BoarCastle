#pragma once

// builtin
#include <cstdint>
#include <iostream>
#include <cmath>
#include <ostream>

// extern
#include <raylib.h>
#include <sys/types.h>


// TODO: tornar métodos constantes em [[nodiscard]]
// TODO: remover "inlines" desnecessários
// TODO: substituir métodos genéricos por funções de conversão

namespace boar {

    template <class NumberT>
    class CustomVector2 {

    public:

        NumberT x;
        NumberT z;

    public:

        constexpr CustomVector2(NumberT const x = 0, NumberT const z = 0): x{x}, z{z} {}

    public:

        inline CustomVector2<NumberT> operator+(CustomVector2<NumberT> const other) const {
            return CustomVector2<NumberT>{this->x + other.x, this->z + other.z};
        }

        inline CustomVector2<NumberT> operator-(CustomVector2<NumberT> const other) const {
            return CustomVector2<NumberT>{this->x - other.x, this->z - other.z};
        }

        inline CustomVector2<NumberT> operator*(CustomVector2<NumberT> const other) const {
            return CustomVector2<NumberT>{this->x * other.x, this->z * other.z};
        }

        inline CustomVector2<NumberT> operator/(CustomVector2<NumberT> const other) const {
            return CustomVector2<NumberT>{this->x / other.x, this->z / other.z};
        }

        inline bool operator==(CustomVector2<NumberT> const other) const {
            return this->x == other.x && this->z == other.z;
        }

        inline bool operator!=(CustomVector2<NumberT> const other) const {
            return this->x != other.x || this->z != other.z;
        }

        inline int32_t manhattan_distance(CustomVector2<NumberT> const other) const {
            return abs(static_cast<int16_t>(this->x) - static_cast<int16_t>(other.x)) +
                   abs(static_cast<int16_t>(this->z) - static_cast<int16_t>(other.z));
        }
    };

    template <class NumberT>
    inline std::ostream& operator<<(std::ostream& ostream, boar::CustomVector2<NumberT> const& vector) {
        return ostream << '(' << vector.x << ',' << vector.z << ")\n";
    }

    using IndexVector2 = CustomVector2<int32_t>;

    template <class NumberT>
    class CustomVector3 {

    public:

        NumberT x = 0;
        NumberT y = 0;
        NumberT z = 0;

    public:

        constexpr CustomVector3() {}
        constexpr CustomVector3(Vector3 const other): x{other.x}, y{other.y}, z{other.z} {}
        constexpr CustomVector3(NumberT const x, NumberT const y, NumberT const z): x{x}, y{y}, z{z} {}

    public:

        template <typename OtherNumberT>
        inline bool operator==(CustomVector3<OtherNumberT> const& other) const {
            return this->x == other.x && this->y == other.y && this->z == other.z;
        }

        inline CustomVector3<NumberT> operator+(CustomVector3<NumberT> const& other) const {
            return CustomVector3<NumberT>(this->x + other.x, this->y + other.y, this->z + other.z);
        }

        inline CustomVector3<NumberT>& operator+=(CustomVector3<NumberT> const& other) {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
            return *this;
        }

        inline CustomVector3<NumberT> operator-(CustomVector3<NumberT> const& other) const {
            return CustomVector3<NumberT>(this->x - other.x, this->y - other.y, this->z - other.z);
        }

        inline CustomVector3<NumberT> operator-() const {
            return CustomVector3<NumberT>(-this->x, -this->y, -this->z);
        }

        inline CustomVector3<NumberT> operator*(CustomVector3<NumberT> const& other) const {
            return CustomVector3<NumberT>(this->x * other.x, this->y * other.y, this->z * other.z);
        }

        template <typename OtherNumberT>
        inline CustomVector3<NumberT> operator*(OtherNumberT const& number) const {
            return CustomVector3<NumberT>(this->x * number, this->y * number, this->z * number);
        }

        inline CustomVector3<NumberT> operator/(CustomVector3<NumberT> const& other) const {
            return CustomVector3<NumberT>(this->x / other.x, this->y / other.y, this->z / other.z);
        }

        template <typename OtherNumberT>
        inline CustomVector3<NumberT> operator/(OtherNumberT const& number) const {
            return CustomVector3<NumberT>(this->x / number, this->y / number, this->z / number);
        }

        inline double distance_to(CustomVector3<NumberT> const& other) const {
            return (*this - other).length();
        }

        inline double length() const {
            return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
        }

        inline CustomVector3<int32_t> to_index(int32_t const step_size = 1) const {
            return CustomVector3<int32_t>{
                static_cast<int32_t>(this->x) / step_size,
                0,
                static_cast<int32_t>(this->z) / step_size,
            };
        }

        inline CustomVector2<int32_t> to_index2(int32_t const step_size = 1) const {
            return CustomVector2<int32_t>{
                static_cast<int32_t>(this->x) / step_size,
                static_cast<int32_t>(this->z) / step_size,
            };
        }

        inline CustomVector3<double> to_global_center(double const step_size) const {
            return CustomVector3<double>{
                static_cast<double>(this->x) * step_size + step_size / 2,
                static_cast<double>(this->y) * step_size,
                static_cast<double>(this->z) * step_size + step_size / 2,
            };
        }

        inline CustomVector3<NumberT> normalized() const {
            double const length = this->length();
            return CustomVector3<NumberT>{
                static_cast<NumberT>(this->x / length),
                static_cast<NumberT>(this->y / length),
                static_cast<NumberT>(this->z / length),
            };
        }

        inline operator Vector3() const {
            return Vector3{
                static_cast<float>(this->x),
                static_cast<float>(this->y),
                static_cast<float>(this->z),
            };
        }

        inline CustomVector2<int32_t> index_vector() const {
            return IndexVector2{this->x, this->z};
        }
    };

    using Vector3u16 = boar::CustomVector3<uint16_t>;
    using Vector3u32 = boar::CustomVector3<int32_t>;
    using Vector3i16 = boar::CustomVector3<int16_t>;
    using Vector3i32 = boar::CustomVector3<int32_t>;
    using Vector3d = boar::CustomVector3<double>;
    using Vector3f = boar::CustomVector3<float>;
    using IndexVector3 = Vector3u32;

} // namespace boar

namespace Vectors {
    inline boar::Vector3u32 ZERO{0, 0, 0};
    inline boar::Vector3u32 ONE{1, 1, 1};
} // namespace Vectors

template <class NumberT>
inline std::ostream& operator<<(std::ostream& ostream, boar::CustomVector3<NumberT> const& vector) {
    return ostream << '(' << vector.x << ',' << vector.y << ',' << vector.z << ")\n";
}

template <class NumberT>
inline Vector3 operator+(Vector3 const vector, boar::CustomVector3<NumberT> const& other) {
    return Vector3{static_cast<float>(vector.x + other.x), static_cast<float>(vector.y + other.y),
                   static_cast<float>(vector.z + other.z)};
}

inline Vector3 operator+(Vector3 const vector, Vector3 other) {
    return Vector3{static_cast<float>(vector.x + other.x), static_cast<float>(vector.y + other.y),
                   static_cast<float>(vector.z + other.z)};
}

template <class NumberT>
inline Vector3 operator+=(Vector3 vector, boar::CustomVector3<NumberT> const& other) {
    vector.x += other.x;
    vector.y += other.y;
    vector.z += other.z;
    return static_cast<Vector3>(vector);
}

template <class NumberT>
inline Vector3 operator*(Vector3 const vector, NumberT const multiplier) {
    return Vector3{static_cast<float>(vector.x * multiplier), static_cast<float>(vector.y * multiplier),
                   static_cast<float>(vector.z * multiplier)};
}
