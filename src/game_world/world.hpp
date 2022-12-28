#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <memory>

#include "../vector.hpp"

class Wall;
class CollisionManager;

class MapTile
{
public:
    //Pathfinding
    std::array<std::shared_ptr<MapTile>,8> neighbors{};
    std::shared_ptr<MapTile> parent;
    double movement_cost = 0;
    double distance_cost = 0;
    double total_cost = 0;
    
    bool pathfinding_started = false;
    bool visited = false;

    bool walkable = true;

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
    enum InputMode
    {
        CONSTRUCTION,
        COMMAND
    };

    constexpr static const boar::IndexVector2 SIZE{600,600};
    constexpr static size_t DIR_COUNT = 8;
    constexpr static uint32_t LINEAR_DIST = 10;
    constexpr static uint32_t DIAGONAL_DIST = 14;
    constexpr static std::array<uint32_t,DIR_COUNT> MOVEMENT_COST
    {
        DIAGONAL_DIST, LINEAR_DIST, DIAGONAL_DIST,
        LINEAR_DIST,                LINEAR_DIST,
        DIAGONAL_DIST, LINEAR_DIST, DIAGONAL_DIST
    };
    

    std::array<std::array<std::shared_ptr<MapTile>, SIZE.z>, SIZE.x> map{};
    InputMode current_input_mode;



    std::shared_ptr<CollisionManager> collision_manager;


    World();

    void add_wall(std::shared_ptr<Wall> wall);
    Path get_path(const boar::IndexVector2 origin, const boar::IndexVector2 target);
    
    void update();
    void render() const;


private:
    std::vector<std::shared_ptr<Wall>> walls;

    void reset_pathfinding();
    Path construct_path(const boar::IndexVector2 start_index, const std::shared_ptr<MapTile> target_tile);
    void set_tile_neighbors(std::shared_ptr<MapTile> tile);

};

inline World game_world;