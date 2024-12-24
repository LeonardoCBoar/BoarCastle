#pragma once

// local
#include "../managers/camera.hpp"
#include "../managers/pod/input_data.hpp"
#include "../utils/vector.hpp"

class InputHandler
{
public:

    InputHandler(const HoverCamera* const camera);

    InputData get_input();

private:

    const HoverCamera* const camera;
};