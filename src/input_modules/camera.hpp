#pragma once

// builtin
#include <cstdlib>
#include <ostream>

// extern
#include <raylib.h>

// local
#include "../utils/vector.hpp"



enum Direction {
    FRONT,
    BACK,
    RIGHT,
    LEFT
};

enum Orientation {
    CLOCKWISE,
    COUNTER_CLOCKWISE,
};

class HoverCamera : public Camera3D {

public:

    double HOVER_SPEED = 50;
    double LIFT_SPEED = 600;
    double const ROTATION_SPEED = PI / 6;
    double const MAX_ZOOM_OUT = 300;

    double current_camera_angle = 0;

    boar::Vector3d current_mouse_pos{};
    boar::IndexVector2 current_mouse_index{};

public:

    HoverCamera(boar::Vector3d const position = {0, 0, 0});

public:

    void update(float const delta_t);

private:

    boar::Vector3d get_ground_intersection_point() const;

    void hover_camera(Direction dir_index, double const delta_t);
    void rotate_camera(double const angle_delta, double const delta_t);
    void lift_camera(double const movement, double const delta_t);

    void set_camera_angle(double const new_angle);
    boar::Vector3d const get_current_dir() const;
};