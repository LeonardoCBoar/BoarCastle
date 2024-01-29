#pragma once

// builtin
#include <map>
#include <memory>
#include <vector>

// local
#include "../utils/vector.hpp"
#include "pod/construction_order.hpp"
#include "pod/input_data.hpp"


const unsigned char MIN_CONSTRUCTION_TRANSPARENCY = 125;
const unsigned char MAX_CONSTRUCTION_TRANSPARENCY = 255;

class ConstructionManager
{
private:

    std::shared_ptr<Wall> preview_wall;

    size_t order_id = 0;
    std::map<size_t, ConstructionOrder> construction_queue;

public:

    ConstructionManager();

    void update(const InputData& input_data);
    void render() const;

    boar::IndexVector2 get_construction_spot(std::shared_ptr<ConstructionOrder> construction_order) const;

private:

    void create_preview_wall(boar::IndexVector2 const mouse_index = boar::IndexVector2{0, 0});
    void assign_order(ConstructionOrder& order);
    void handle_input(const InputData& input_data);
};