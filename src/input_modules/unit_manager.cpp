#include "unit_manager.hpp"

#include <cstdint>
#include <iostream>

#include "raylib.h"

#include "../input_modules/camera.hpp"
#include "../game_world/world.hpp"
#include "../utils/utils.hpp"

UnitMananger::UnitMananger(const HoverCamera* const camera)
    :camera{camera}
{

}

void UnitMananger::update()
{
    if(game_world.current_input_mode != World::InputMode::COMMAND) return;

    const bool is_inside_borders = game_world.is_inside_borders(camera->current_mouse_index);

    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && is_inside_borders)
    {
        this->start_point = camera->current_mouse_index;
        if(this->start_point != this->target_point)
        {
            TimeMeasurer path_measurer{"Found path in "};
            this->path = game_world.get_path(this->start_point, this->target_point);
            path_measurer.print_time();
        }

    }
    else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && is_inside_borders)
    {

        this->target_point = camera->current_mouse_index;
        if(this->start_point != this->target_point)
            this->path = game_world.get_path(this->start_point, this->target_point);
    }

    if(IsKeyDown('T'))
    {
        auto total_time = 0;
        std::cout << "PF test\n";
        boar::IndexVector2 origin{0,0};
        for(int32_t x = 0; x < 300; x+=30)
        {

            for(int32_t z = 0; z < 300; z+=30)
            {
                boar::IndexVector2 target{x,z};
                std::cout << target;
                TimeMeasurer pathtimer{"Path found in"};
                this->path = game_world.get_path(origin, target);
                total_time += pathtimer.get_time();
                pathtimer.print_time();

            }
        }
        std::cout << "Found all 100 paths in " << total_time/1000 << "." << total_time%1000 << "s\n";
    }
    
}

void UnitMananger::render() const
{
    for(const auto& tile : this->path)
    {
        Vector3 pos{};
        pos.x = tile.x + 0.5;
        pos.y = 0;
        pos.z = tile.z + 0.5;
        DrawCube(pos, 1, 0.1, 1, GREEN);
    }
    Vector3 start_pos{};
    start_pos.x = start_point.x + 0.5;
    start_pos.y = 0;
    start_pos.z = start_point.z + 0.5;
    DrawCube(start_pos, 1, 0.1, 1, BLUE);
    Vector3 target_pos{};
    target_pos.x = target_point.x + 0.5;
    target_pos.y = 0;
    target_pos.z = target_point.z + 0.5;
    DrawCube(target_pos, 1, 0.1, 1, RED);
}