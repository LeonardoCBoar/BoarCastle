#include <ostream>
#include <raylib.h>

#include "camera.hpp"
#include "game_world/world.hpp"
#include "vector.hpp"
#include "interaction_modules/construction_manager.hpp"
#include <optional>

const int SCREEN_WIDTH  = 1280;
const int SCREEN_HEIGHT = 720;

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BoarCastle");

    HoverCamera camera{ {300, 100, 300} };

    ConstructionManager construction_manager{&camera};

    SetCameraMode(camera, CAMERA_FREE);
    //SetTargetFPS(60);




    while (!WindowShouldClose())
    {
        const double delta_t = GetFrameTime();
        camera.update(delta_t);

        construction_manager.update();

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                //DrawPlane({500,0,500}, {1000,1000}, Color{0,50,0,255});
                DrawGrid(1200/5, 5.0f);

                construction_manager.render();
                game_world.render();

            EndMode3D();

            DrawFPS(10, 10);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}