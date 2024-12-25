#pragma once

// local
#include "../../utils/vector.hpp"

enum TimeInput
{
    TI_NONE,
    DECREASE_SPEED,
    PAUSE,
    INCREASE_SPEED
};

enum MapInput
{
    MP_NONE,
    LEFT_CLICK,
    RIGHT_CLICK,
};

struct InputData
{
    boar::IndexVector2 mouse_index;
    TimeInput time_input;
    MapInput map_input;
    bool shift_down;
};