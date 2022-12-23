#include "construction_manager.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <raylib.h>
#include <type_traits>

#include "../camera.hpp"
#include "../game_world/game_objects/wall.hpp"
#include "../game_world/world.hpp"
#include "../game_world/collision_manager.hpp"


ConstructionManager::ConstructionManager(const HoverCamera* const camera)
    :camera{camera} 
{
    this->create_preview_wall();
}

void ConstructionManager::create_preview_wall(const boar::IndexVector3 mouse_index)
{
    this->preview_wall = std::make_shared<Wall>(mouse_index);
    this->preview_wall->color.a = 125;
}

boar::Vector3d ConstructionManager::get_ground_intersection_point() const
{
    const Ray mouse_ray = GetMouseRay(GetMousePosition(), *camera);

    //Distance: Amount of dir vectors necessary to reach intersection point
    const double distance = (-mouse_ray.position.y) / mouse_ray.direction.y;
    const boar::Vector3d intersection_point = mouse_ray.position + mouse_ray.direction * distance;

    return intersection_point;
}

void ConstructionManager::update()
{
    this->handle_input();
}

void ConstructionManager::handle_input()
{
    const auto mouse_intersection_point = this->get_ground_intersection_point();

    if(!game_world.collision_manager->is_inside_borders(mouse_intersection_point))
    {
        this->preview_wall->visible = false;
        return;
    }

    this->preview_wall->visible = true;
    const auto selected_tile = mouse_intersection_point.to_index();
    this->preview_wall->move_to(selected_tile);


    const boar::Vector3u32 half_wall_size = preview_wall->SIZE/2;
    bool free_space = game_world.collision_manager->can_fit_object(preview_wall);


    if(free_space)
    {
        preview_wall->color = GRAY;
        preview_wall->color.a = 125;
    }
    else 
    {
        preview_wall->color = RED;
        preview_wall->color.a = 255;
    }

    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && free_space)
    {
        this->preview_wall->color = GRAY;
        game_world.add_wall(this->preview_wall);
        this->create_preview_wall(selected_tile);

    }
}

void ConstructionManager::render() const
{
    //Preview Wall
    this->preview_wall->render();
}
