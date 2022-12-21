#include <ostream>
#include <raylib.h>

#include "camera.hpp"
#include "game_world/world.hpp"
#include "vector.hpp"
#include "interaction_modules/construction_manager.hpp"

const int SCREEN_HEIGHT = 666;
const int SCREEN_WIDTH  = 666;

const double CAMERA_SPEED = 10;

int main(void)
{
    InitWindow(1280, 720, "BoarCastle");

    HoverCamera camera{ {500, 100, 500} };

    ConstructionManager construction_manager{&camera};

    SetCameraMode(camera, CAMERA_FREE);



    while (!WindowShouldClose())
    {
        const double delta_t = GetFrameTime();
        camera.update(delta_t);

        construction_manager.update();

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                //DrawPlane({500,0,500}, {1000,1000}, Color{0,50,0,255});
                DrawGrid(2000, 5.0f);

                construction_manager.render();
                game_world.render();

            EndMode3D();

            DrawFPS(10, 10);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}