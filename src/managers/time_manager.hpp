#pragma once

// builtin
#include <chrono>

class TimeManager
{
private:
    std::chrono::system_clock::time_point current_frame_start;
    double frame_time;
public:
    TimeManager();
    void start_frame();
    double get_frame_time() const;
};