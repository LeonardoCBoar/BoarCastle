#pragma once

// local
#include "../utils/vector.hpp"
#include "../managers/pod/input_data.hpp"
#include "../managers/camera.hpp"

class InputHandler
{
public:
    InputHandler(const HoverCamera* const camera);

    InputData get_input();

private:
    const HoverCamera* const camera;
};