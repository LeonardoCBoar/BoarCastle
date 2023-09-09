
// builtin
#include <memory>

// local
#include "../utils/vector.hpp"



class HoverCamera;
class Wall;

class ConstructionManager {

private:

    HoverCamera const* const camera;
    std::shared_ptr<Wall> preview_wall;

public:

    ConstructionManager(HoverCamera const* const camera);

public:

    void update();
    void render() const;

private:

    void create_preview_wall(boar::IndexVector2 const mouse_index = boar::IndexVector2{0, 0});
    void handle_input();
};