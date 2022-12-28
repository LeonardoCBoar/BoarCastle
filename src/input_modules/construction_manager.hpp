#include <memory>

#include "../utils/vector.hpp"

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
    void create_preview_wall(const boar::IndexVector2 mouse_index = boar::IndexVector2{0,0});
    void handle_input();

};