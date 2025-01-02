#pragma once

// builtin
#include <optional>
#include <vector>

// local
#include "../game_world/game_objects/worker.hpp"
#include "../utils/rect.hpp"
#include "../utils/vector.hpp"
#include "pod/input_data.hpp"



class HoverCamera;

class UnitManager
{
public:

    UnitManager();

    void update(const float delta, const InputData& input_data);
    void render_3d() const;
    void render_2d() const;


public:

    std::vector<Worker> workers;

private:

    void handle_input(const InputData& input_data);

    Worker* get_selected_worker();
    const Worker* get_selected_worker() const;

private:

    uint64_t next_worker_id = 0;
    std::optional<uint64_t> selected_worker_id = std::nullopt;
    std::vector<uint64_t> selected_workers;

    std::optional<Rect> selection_rect = std::nullopt;
};