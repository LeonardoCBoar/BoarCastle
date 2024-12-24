#include "time_handler.hpp"

#include <raylib.h>

TimeHandler::TimeHandler()
{
}

void TimeHandler::update(const InputData input_data)
{
    switch(input_data.time_input)
    {
        case TimeInput::DECREASE_SPEED:
        {
            if(speed > 1)
                this->speed -= 1;
            else if(speed > 0.15)
                this->speed -= 0.1;
            std::cout << "Decreased speed to: " << this->speed << std::endl;
            break;
        }
        case TimeInput::INCREASE_SPEED:
        {
            if(speed >= 1)
                this->speed += 1;
            else if(speed > 0)
                this->speed += 0.1;
            std::cout << "Increased speed to: " << this->speed << std::endl;
            break;
        }
        case TimeInput::PAUSE:
        {
            this->paused = !this->paused;
            std::cout << "paused " << std::endl;
            break;
        }
        default:
            break;
    }
}

double TimeHandler::get_frame_time(const bool apply_speed) const
{
    if(this->paused)
        return 0;
    
    const double frame_time = GetFrameTime();
    if(!apply_speed)
        return frame_time;

    return frame_time * this->speed;
}