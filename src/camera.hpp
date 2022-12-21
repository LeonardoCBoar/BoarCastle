#pragma once
#include <ostream>
#include <stdlib.h>

#include <raylib.h>

#include "vector.hpp"

enum Direction
{
    FRONT,
    BACK,
    RIGHT,
    LEFT
};

enum Orientation
{
    CLOCKWISE,
    COUNTER_CLOCKWISE,
};

class HoverCamera : public Camera3D
{
    double HOVER_SPEED = 50;
    double LIFT_SPEED = 300;
    const double ROTATION_SPEED = PI/6;
    const double MAX_ZOOM_OUT = 300;

    double current_camera_angle = 0;

    
public:
    HoverCamera(const boar::Vector3d position = {0,0,0});

    void update(const float delta_t);

private:
    void hover_camera(Direction dir_index, const double delta_t);
    void rotate_camera(const double angle_delta, const double delta_t);
    void lift_camera(const double movement, const double delta_t);

    void set_camera_angle(const double new_angle);
    const boar::Vector3d get_current_dir() const;

};