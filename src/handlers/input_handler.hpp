#pragma once

// local
#include "../managers/camera.hpp"
#include "../managers/pod/input_data.hpp"
#include "../utils/rect.hpp"
#include "../utils/vector.hpp"

class InputHandler
{
public:

    InputHandler(const Rect screen_rect, const HoverCamera* const camera);

    void handle_input();
    InputData get_input() const;

private:

    TimeInput handle_time_input(const bool shift_down) const;
    MouseInput handle_mouse_input(const bool shift_down) const;

    const Rect screen_rect;
    const HoverCamera* const camera;
    InputData last_input;
};