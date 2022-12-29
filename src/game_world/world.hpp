#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <memory>

#include "../utils/vector.hpp"

class Wall;
class CollisionManager;

class MapTile
{
public:
    //Pathfinding
    std::array<std::shared_ptr<MapTile>,8> neighbors{};
    std::shared_ptr<MapTile> parent;
    uint32_t set_id = 0;
    double movement_cost = 0;
    double distance_cost = 0;
    double total_cost = 0;
    
    bool pathfinding_started = false;
    bool visited = false;


    bool update_set();
    void reset_pathfinding();
    void setup_pathfinding(const boar::IndexVector2 target, const std::shared_ptr<MapTile> parent, const int parent_dir_index);
    //

    boar::IndexVector2 index;

    MapTile(const boar::IndexVector2 index);


};

typedef std::vector<boar::IndexVector2> Path ;

class World
{
public:
    constexpr static const boar::IndexVector2 SIZE{600,600};

private:
    std::array<std::array<std::shared_ptr<MapTile>, SIZE.z>, SIZE.x> map{};
    std::vector<std::shared_ptr<Wall>> walls;

    void set_tile_neighbors(std::shared_ptr<MapTile> tile);
    Path construct_path(const boar::IndexVector2 start_index, const std::shared_ptr<MapTile> target_tile);
    void update_tile_sets();
    void reset_pathfinding();

public:
    enum InputMode
    {
        CONSTRUCTION,
        COMMAND
    };

    constexpr static size_t DIR_COUNT = 8;
    constexpr static uint32_t LINEAR_DIST = 10;
    constexpr static uint32_t DIAGONAL_DIST = 14;
    constexpr static std::array<uint32_t,DIR_COUNT> MOVEMENT_COST
    {
        DIAGONAL_DIST, LINEAR_DIST, DIAGONAL_DIST,
        LINEAR_DIST,                LINEAR_DIST,
        DIAGONAL_DIST, LINEAR_DIST, DIAGONAL_DIST
    };
    

    InputMode current_input_mode;



    std::shared_ptr<CollisionManager> collision_manager;


    World();

    Path get_path(const boar::IndexVector2 origin, const boar::IndexVector2 target);
    std::shared_ptr<MapTile> get_tile(const boar::IndexVector2);
    void add_wall(std::shared_ptr<Wall> wall);
    
    void update();
    void render() const;



};

inline World game_world;