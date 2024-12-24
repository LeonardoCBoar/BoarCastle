#pragma once

// builtin
#include <chrono>

// local
#include "../utils/utils.hpp"
#include "../managers/pod/input_data.hpp"



class TimeHandler
{
private:
public:
    TimeHandler();

    void update(const InputData input_data);
    double get_frame_time(const bool apply_speed = true) const;

    double speed = 1.0;
    bool paused = false;
};