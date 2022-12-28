#pragma once

#include "../utils/vector.hpp"
#include <vector>

class HoverCamera;

class UnitMananger
{
public:
    UnitMananger(const HoverCamera* const camera);

    boar::IndexVector2 start_point{};
    boar::IndexVector2 target_point{};

    std::vector<boar::IndexVector2> path{};

    void update();
    void render() const;

private:
    const HoverCamera* const camera;
};