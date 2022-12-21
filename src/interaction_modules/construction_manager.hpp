#include <memory>

#include "../vector.hpp"

class HoverCamera;
class Wall;

class ConstructionManager
{
private:
    const HoverCamera* const camera;

    boar::Vector3u32 selected_tile;
    std::shared_ptr<Wall> preview_wall;

public:
    ConstructionManager(const HoverCamera* const camera);

    void update();
    void render() const;

private:
    void create_preview_wall();
    boar::Vector3u32 get_ground_intersection_index() const;
    void handle_input();

};