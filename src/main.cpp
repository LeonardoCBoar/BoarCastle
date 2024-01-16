
// extern
#include <raylib.h>

// local
#include "game_world/world.hpp"
#include "input_modules/camera.hpp"
#include "input_modules/construction_manager.hpp"
#include "input_modules/unit_manager.hpp"



int const SCREEN_WIDTH = 1280;
int const SCREEN_HEIGHT = 720;

int main(void)
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BoarCastle");
    HoverCamera camera{{10, 50, 10}};

    game_world.initialize_modules(&camera);

    SetTargetFPS(60);
    UpdateCamera(&camera, CAMERA_FREE);

    while (!WindowShouldClose()) {
        float const delta_t = GetFrameTime();
        camera.update(delta_t);
        game_world.update();

        game_world.construction_manager->update();
        game_world.unit_manager->update(delta_t);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        // DrawPlane({500,0,500}, {1000,1000}, Color{0,50,0,255});
        DrawGrid(1200 / 2, 2.0f);

        game_world.render();
        game_world.construction_manager->render();
        game_world.unit_manager->render();

        EndMode3D();

        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}