
// extern
#include <raylib.h>

// local
#include "game_world/world.hpp"
#include "managers/camera.hpp"
#include "managers/construction_manager.hpp"
#include "managers/pod/input_data.hpp"
#include "managers/unit_manager.hpp"



const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
std::string const WINDOW_NAME = "BoarCastle";

int main(void)
{
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME.c_str());
    HoverCamera camera{{10, 50, 10}};

    SetTargetFPS(60);
    UpdateCamera(&camera, CAMERA_FREE);

    while (!WindowShouldClose())
    {
        const float delta_t = GetFrameTime();
        InputData input_data;

        camera.update(delta_t);
        input_data.mouse_index = camera.current_mouse_index;
        game_world.update();

        game_world.construction_manager->update(input_data);
        game_world.unit_manager->update(delta_t, input_data);

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