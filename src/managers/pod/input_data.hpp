#pragma once

// local
#include "../../utils/vector.hpp"

enum TimeInput
{
    NONE,
    DECREASE_SPEED,
    PAUSE,
    INCREASE_SPEED
};

struct InputData
{
    boar::IndexVector2 mouse_index;
    TimeInput time_input;
};