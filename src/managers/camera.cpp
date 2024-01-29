// header
#include "camera.hpp"

// local
#include "../utils/vector.hpp"



HoverCamera::HoverCamera(boar::Vector3d const position)
{
    this->position = position;
    this->up = boar::Vector3d{0, 1, 0};
    this->fovy = 45;
    this->projection = CAMERA_PERSPECTIVE;

    this->set_camera_angle(this->current_camera_angle);
}

void HoverCamera::update(const float delta_t)
{
    boar::Vector3d current_mouse_pos = this->get_ground_intersection_point();
    this->current_mouse_index = current_mouse_pos.to_index2();

    if (IsKeyDown('W') || IsKeyDown(KEY_UP))
        this->hover_camera(Direction::FRONT, delta_t);
    if (IsKeyDown('S') || IsKeyDown(KEY_DOWN))
        this->hover_camera(Direction::BACK, delta_t);
    if (IsKeyDown('A') || IsKeyDown(KEY_LEFT))
        this->hover_camera(Direction::LEFT, delta_t);
    if (IsKeyDown('D') || IsKeyDown(KEY_RIGHT))
        this->hover_camera(Direction::RIGHT, delta_t);

    if (IsKeyDown('Q'))
        this->rotate_camera(-ROTATION_SPEED, delta_t);
    if (IsKeyDown('E'))
        this->rotate_camera(ROTATION_SPEED, delta_t);

    this->lift_camera(-GetMouseWheelMove(), delta_t);
}

boar::Vector3d HoverCamera::get_ground_intersection_point() const
{
    const Ray mouse_ray = GetMouseRay(GetMousePosition(), *this);

    // Distance: Amount of dir vectors necessary to reach intersection point
    const double distance = (-mouse_ray.position.y) / mouse_ray.direction.y;
    boar::Vector3d const intersection_point = mouse_ray.position + mouse_ray.direction * distance;

    return intersection_point;
}


void HoverCamera::hover_camera(Direction dir_index, const double delta_t)
{
    boar::Vector3d const current_dir = this->get_current_dir();
    boar::Vector3d movement_dir = current_dir;

    switch (dir_index)
    {
        case Direction::FRONT:
            movement_dir = current_dir;
            break;
        case Direction::BACK:
            movement_dir = -current_dir;
            break;
        case Direction::LEFT:
            movement_dir.x = current_dir.z;
            movement_dir.z = -current_dir.x;
            break;
        case Direction::RIGHT:
            movement_dir.x = -current_dir.z;
            movement_dir.z = current_dir.x;
            break;
    }

    this->position = this->position + movement_dir * delta_t * HOVER_SPEED;
    this->set_camera_angle(this->current_camera_angle);
}

void HoverCamera::rotate_camera(const double angle_delta, const double delta_t)
{
    const double new_angle = this->current_camera_angle + (angle_delta * delta_t);
    this->set_camera_angle(new_angle);
}

void HoverCamera::lift_camera(const double movement, const double delta_t)
{
    if (movement != 0)
    {
        this->position.y += movement * LIFT_SPEED * delta_t;
        if (this->position.y > this->MAX_ZOOM_OUT)
            this->position.y = MAX_ZOOM_OUT;
        else if (this->position.y < 1)
            this->position.y = 1;

        this->set_camera_angle(this->current_camera_angle);
    }
}

void HoverCamera::set_camera_angle(const double new_angle)
{
    this->current_camera_angle = new_angle;
    boar::Vector3d const looking_dir{static_cast<double>(cos(new_angle)), -1, static_cast<double>(sin(new_angle))};

    this->target = this->position + looking_dir;
}

boar::Vector3d const HoverCamera::get_current_dir() const
{
    return boar::Vector3d{
        this->target.x - this->position.x,
        0,
        this->target.z - this->position.z,
    };
}