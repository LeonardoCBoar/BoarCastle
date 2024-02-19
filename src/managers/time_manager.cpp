#include "time_manager.hpp"

TimeManager::TimeManager()
{
    this->start_frame();
}

void TimeManager::start_frame()
{
    auto last_frame_start = this->current_frame_start;
    this->current_frame_start = std::chrono::high_resolution_clock::now();

    auto integer_frame_time = std::chrono::duration_cast<std::chrono
    this->frame_time = 

}

double TimeManager::get_frame_time() const
{
    return this->frame_time;
}