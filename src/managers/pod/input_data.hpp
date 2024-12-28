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

enum MouseInputType
{
    MI_NONE,
    LEFT_CLICK,
    SHIFT_LEFT_CLICK,
    LEFT_CLICK_SELECTION,
    RIGHT_CLICK,
};

struct MouseInput
{
    MouseInputType type;
    boar::IndexVector2 pos;
    boar::IndexVector2 index;
    boar::IndexVector2 selection_start_index;
    boar::IndexVector2 selection_start_pos;
};


struct InputData
{
    TimeInput time_input;
    MouseInput mouse_input;
};