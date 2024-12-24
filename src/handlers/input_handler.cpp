#include "input_handler.hpp"

#include <raylib.h>

InputHandler::InputHandler(const HoverCamera* const camera)
    :camera{camera}
    {

    }

InputData InputHandler::get_input()
{
    InputData input_data;
    const bool shift_down = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

    input_data.mouse_index = this->camera->current_mouse_index;

    if((IsKeyPressed(KEY_EQUAL) && shift_down) || IsKeyPressed(KEY_KP_ADD))
        input_data.time_input = TimeInput::INCREASE_SPEED;
    else if (IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT))
        input_data.time_input = TimeInput::DECREASE_SPEED;
    else if(IsKeyPressed(KEY_SPACE))
        input_data.time_input = TimeInput::PAUSE;
    else
        input_data.time_input = TimeInput::NONE;
    
    return input_data;
}