#include <memory>

#include "../vector.hpp"

class HoverCamera;
class Wall;

class ConstructionManager
{
private:
    const HoverCamera* const camera;

    std::shared_ptr<Wall> preview_wall;

public:
    ConstructionManager(const HoverCamera* const camera);

    void update();
    void render() const;

private:
    void create_preview_wall(const boar::IndexVector3 mouse_index = boar::IndexVector3{0,0,0});
    boar::Vector3d get_ground_intersection_point() const;
    void handle_input();

};