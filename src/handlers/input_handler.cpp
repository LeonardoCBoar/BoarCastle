#include "input_handler.hpp"

// extern
#include <raylib.h>

InputHandler::InputHandler(const Rect screen_rect, const HoverCamera* const camera):
    screen_rect{screen_rect}, camera{camera}
{
}

void InputHandler::handle_input()
{
    const bool shift_down = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

    InputData input_data;
    input_data.time_input = this->handle_time_input(shift_down);
    input_data.mouse_input = this->handle_mouse_input(shift_down);

    this->last_input = input_data;
}

TimeInput InputHandler::handle_time_input(const bool shift_down) const
{
    TimeInput time_input;

    if ((IsKeyPressed(KEY_EQUAL) && shift_down) || IsKeyPressed(KEY_KP_ADD))
        time_input = TimeInput::INCREASE_SPEED;
    else if (IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT))
        time_input = TimeInput::DECREASE_SPEED;
    else if (IsKeyPressed(KEY_SPACE))
        time_input = TimeInput::PAUSE;
    else
        time_input = TimeInput::TI_NONE;

    return time_input;
}

MouseInput InputHandler::handle_mouse_input(const bool shift_down) const
{
    MouseInput mouse_input;
    mouse_input.index = this->camera->current_mouse_index;

    Vector2 mouse_pos = GetMousePosition();
    mouse_input.pos.x = mouse_pos.x;
    mouse_input.pos.z = mouse_pos.y;

    const bool is_inside_borders = mouse_input.index.is_inside_rect(this->screen_rect);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && is_inside_borders)
    {
        mouse_input.type = shift_down ? MouseInputType::SHIFT_LEFT_CLICK : MouseInputType::LEFT_CLICK;
        mouse_input.selection_start_index = mouse_input.index;
        mouse_input.selection_start_pos = mouse_input.pos;
    }
    else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
             (this->last_input.mouse_input.type == MouseInputType::LEFT_CLICK ||
              this->last_input.mouse_input.type == MouseInputType::LEFT_CLICK_SELECTION))
    {
        mouse_input.type = MouseInputType::LEFT_CLICK_SELECTION;
        mouse_input.selection_start_index = this->last_input.mouse_input.selection_start_index;
        mouse_input.selection_start_pos = this->camera->grid_index_to_mouse_pos(mouse_input.selection_start_index);
    }
    else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && is_inside_borders)
        mouse_input.type = MouseInputType::RIGHT_CLICK;
    else
        mouse_input.type = MouseInputType::MI_NONE;

    return mouse_input;
}

InputData InputHandler::get_input() const
{
    return this->last_input;
}
