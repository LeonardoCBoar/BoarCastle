#pragma once

// builtin
#include <algorithm>
#include <cstdlib>

class Rect
{
public:

    int32_t pos_x;
    int32_t pos_z;
    int32_t size_x;
    int32_t size_z;

    Rect(const int32_t pos_x, const int32_t pos_z, const int32_t size_x, const int32_t size_z):
        pos_x{pos_x}, pos_z{pos_z}, size_x{size_x}, size_z{size_z}
    {
    }

    static Rect from_unoredered_points(const int32_t x1, const int32_t z1, const int32_t x2, const int32_t z2)
    {
        const int32_t pos_x = std::min(x1, x2);
        const int32_t pos_z = std::min(z1, z2);
        const int32_t size_x = std::abs(x1 - x2);
        const int32_t size_z = std::abs(z1 - z2);

        return Rect(pos_x, pos_z, size_x, size_z);
    }
};

inline std::ostream& operator<<(std::ostream& ostream, const Rect rect)
{
    return ostream << "top left: (" << rect.pos_x << ", " << rect.pos_z << ")\nbottom right: ("
                   << (rect.pos_x + rect.size_x) << ", " << (rect.pos_z + rect.size_z) << ")" << std::endl;
};
