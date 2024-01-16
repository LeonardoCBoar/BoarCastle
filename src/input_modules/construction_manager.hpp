#pragma once

// builtin
#include <memory>
#include <vector>
#include <map>

// local
#include "../utils/vector.hpp"
#include "construction_order.hpp"

class HoverCamera;

const unsigned char MIN_CONSTRUCTION_TRANSPARENCY = 125;
const unsigned char MAX_CONSTRUCTION_TRANSPARENCY = 255;

class ConstructionManager 
{

private:

    HoverCamera const* const camera;
    std::shared_ptr<Wall> preview_wall;

    size_t order_id = 0;
    std::map<size_t, ConstructionOrder> construction_queue;

public:
    ConstructionManager(HoverCamera const* const camera);

    void update();
    void render() const;

    boar::IndexVector2 get_construction_spot(std::shared_ptr<ConstructionOrder> construction_order) const;

private:

    void create_preview_wall(boar::IndexVector2 const mouse_index = boar::IndexVector2{0, 0});
    void assign_order(ConstructionOrder& order);
    void handle_input();
};