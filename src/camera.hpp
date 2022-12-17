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
    const double HOVER_SPEED = 10;
    const double LIFT_SPEED = 300;
    const double ROTATION_SPEED = PI/6;
    double current_camera_angle = 0;
    
public:
    HoverCamera(const boar::Vector3f position = {0,0,0})
    {
        this->position = position;
        this->up = boar::Vector3f{0,1,0};
        this->fovy = 45;
        this->projection = CAMERA_PERSPECTIVE;

        this->set_camera_angle(this->current_camera_angle);
    }

    void update(const float delta)
    {
        if(IsKeyDown('W') || IsKeyDown(KEY_UP))
            this->hover_camera( Direction::FRONT);
        if(IsKeyDown('S') || IsKeyDown(KEY_DOWN))
            this->hover_camera( Direction::BACK);
        if(IsKeyDown('A') || IsKeyDown(KEY_LEFT))
            this->hover_camera( Direction::LEFT);
        if(IsKeyDown('D') || IsKeyDown(KEY_RIGHT))
            this->hover_camera( Direction::RIGHT);
        
        if(IsKeyDown('Q'))
            this->rotate_camera(-ROTATION_SPEED * GetFrameTime());
        if(IsKeyDown('E'))
            this->rotate_camera(ROTATION_SPEED * GetFrameTime());

        this->lift_camera(-GetMouseWheelMove());
    }

private:
    void hover_camera(Direction dir_index)
    {   
        const boar::Vector3f current_dir = this->get_current_dir();
        boar::Vector3f movement_dir = current_dir;
        
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

        const float delta = GetFrameTime();
        this->position = this->position + movement_dir * delta * HOVER_SPEED;
        this->rotate_camera(0);
    }

    void rotate_camera(const double angle_delta)
    {
        const double new_angle = this->current_camera_angle + (angle_delta);
        this->set_camera_angle(new_angle); 
    }

    void lift_camera(const double movement)
    {
        if(movement != 0)
        {
            this->position.y += movement * LIFT_SPEED * GetFrameTime();
            if(this->position.y > 100) this->position.y = 100;
            else if(this->position.y < 1) this->position.y = 1;

            this->set_camera_angle(this->current_camera_angle);
        }

    }

    void set_camera_angle(const double new_angle)
    {
        this->current_camera_angle = new_angle;
        const boar::Vector3f looking_dir
        {
            static_cast<float>(cos(new_angle)),
            -1,
            static_cast<float>(sin(new_angle))
        };

        this->target = this->position + looking_dir;
    }

    const boar::Vector3f get_current_dir() const
    {
        return boar::Vector3f
        {
            this->target.x - this->position.x,
            0,
            this->target.z - this->position.z,
        };
    }


};