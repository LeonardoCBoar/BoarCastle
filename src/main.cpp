#include <ostream>
#include <raylib.h>

#include "camera.hpp"
#include "vector.hpp"

const int SCREEN_HEIGHT = 666;
const int SCREEN_WIDTH  = 666;

const double CAMERA_SPEED = 10;

int main(void)
{
    InitWindow(800, 450, "BoarCastle");

    HoverCamera camera{ {10, 10, -5} };

    SetCameraMode(camera, CAMERA_FREE);

    boar::Vector3f cube_pos{15.0f,0.0f,2.0f};
    boar::Vector3f cube_pos2{0.0f,0.0f,5.0f};
    boar::Vector3f cube_pos3{7.5f,0.0f,12.0f};

    while (!WindowShouldClose())
    {
        camera.update(GetFrameTime());
        //UpdateCamera(&camera);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawCube(cube_pos, 2.0f, 2.0f, 2.0f, RED);
                DrawCube(cube_pos2, 2.0f, 2.0f, 2.0f, BLUE);
                DrawCube(cube_pos3, 2.0f, 2.0f, 2.0f, GREEN);
                DrawCubeWires(cube_pos, 2.0f, 2.0f, 2.0f, MAROON);
                DrawCubeWires(cube_pos2, 2.0f, 2.0f, 2.0f, MAROON);
                DrawCubeWires(cube_pos3, 2.0f, 2.0f, 2.0f, MAROON);
                DrawGrid(100, 1.0f);

            EndMode3D();

            DrawFPS(10, 10);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}