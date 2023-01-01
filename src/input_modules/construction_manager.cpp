#include "construction_manager.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <raylib.h>
#include <type_traits>

#include "../input_modules/camera.hpp"
#include "../game_world/game_objects/wall.hpp"
#include "../game_world/world.hpp"


ConstructionManager::ConstructionManager(const HoverCamera* const camera)
    :camera{camera} 
{
    this->create_preview_wall();
}

void ConstructionManager::create_preview_wall(const boar::IndexVector2 mouse_index)
{
    this->preview_wall = std::make_shared<Wall>(mouse_index);
    this->preview_wall->color.a = 125;
}

void ConstructionManager::update()
{
    this->handle_input();
}

void ConstructionManager::handle_input()
{
    if(game_world.current_input_mode != World::InputMode::CONSTRUCTION)
        return;

    const auto selected_tile = camera->current_mouse_index;

    if(!game_world.is_inside_borders(selected_tile))
    {
        this->preview_wall->visible = false;
        return;
    }

    this->preview_wall->visible = true;
    this->preview_wall->move_to(selected_tile);


    const boar::Vector3u32 half_wall_size = preview_wall->SIZE/2;
    bool free_space = game_world.can_fit_object(preview_wall);


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
    if(game_world.current_input_mode != World::InputMode::CONSTRUCTION)
        return;
    //Preview Wall
    this->preview_wall->render();
}
