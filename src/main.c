#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "rlgl.h"
#include <stdio.h>
#include <string.h>
#include "worldgeneration.c"
#include "worldmodel.c"

int main(void)
{
    const int screenWidth = 1920 / 2;
    const int screenHeight = 1080 / 2;

    InitWindow(screenWidth, screenHeight, "Raycraft");

    // todo: implement custom camera that fits the game
    Camera camera = {0};
    camera.position = (Vector3){4.0f, 10.0f, 4.0f};
    camera.target = (Vector3){0.0f, 1.8f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.type = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_FIRST_PERSON);

    SetTargetFPS(60);

    int *world = GenerateWorld();
    Model worldModel = GetWorldModel(world);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera);

        BeginDrawing();
        {
            ClearBackground(SKYBLUE);

            BeginMode3D(camera);
            {
                // Clouds
                DrawCube((Vector3){600.0f, 200.0f, 600.0f}, 100.0f, 10.0f, 37.0f, WHITE);
                DrawCube((Vector3){250.0f, 200.0f, 150.0f}, 49.0f, 10.0f, 40.0f, WHITE);
                DrawCube((Vector3){900.0f, 200.0f, 130.0f}, 60.0f, 10.0f, 300.0f, WHITE);
                DrawCube((Vector3){200.0f, 200.0f, -50.0f}, 30.0f, 10.0f, 37.0f, WHITE);
                DrawCube((Vector3){1000.0f, 200.0f, 200.0f}, 30.0f, 10.0f, 37.0f, WHITE);
                DrawCube((Vector3){50.0f, 200.0f, 30.0f}, 30.0f, 10.0f, 37.0f, WHITE);
                // World
                DrawModel(worldModel, (Vector3){0, 0, 0}, 1.0f, WHITE);
            }
            EndMode3D();

            DrawText("Raycraft\nMade by Michal (twitter: @pietmichal)\nPowered by raylib.com", 10, 10, 20, WHITE);
            DrawFPS(1920 / 2 - 90, 10);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}