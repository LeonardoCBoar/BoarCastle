#pragma once

// builtin
#include <vector>
#include <optional>

// local
#include "../game_world/game_objects/worker.hpp"
#include "../utils/vector.hpp"
#include "pod/input_data.hpp"



class HoverCamera;

class UnitManager
{
public:

    std::vector<boar::IndexVector2> path{};
    std::vector<Worker> workers;

    std::optional<uint64_t> selected_worker_id = std::nullopt;
    uint64_t next_worker_id = 0;

public:

    UnitManager();

public:

    void update(const float delta, const InputData& input_data);
    void render() const;

private:
    Worker* get_selected_worker();
    const Worker* get_selected_worker() const;
};