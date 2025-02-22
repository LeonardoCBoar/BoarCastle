#pragma once

// builtin
#include <cstdlib>
#include <ostream>

// extern
#include <raylib.h>

// local
#include "../utils/vector.hpp"



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
private:

    double HOVER_SPEED = 50;
    double LIFT_SPEED = 600;
    const double ROTATION_SPEED = PI / 6;
    const double MAX_ZOOM_OUT = 300;

    double current_camera_angle = 0;

public:

    boar::IndexVector2 current_mouse_index{};

public:

    HoverCamera(const boar::Vector3d position = {0, 0, 0});

    void update(const float delta_t);

    boar::Vector3d get_ground_intersection_point(const Vector2 mouse_position) const;
    boar::IndexVector2 grid_index_to_mouse_pos(const boar::IndexVector2 grid_index) const;

private:


    void hover_camera(const Direction dir_index, const double delta_t);
    void rotate_camera(const double angle_delta, const double delta_t);
    void lift_camera(const double movement, const double delta_t);

    void set_camera_angle(const double new_angle);
    boar::Vector3d const get_current_dir() const;
};