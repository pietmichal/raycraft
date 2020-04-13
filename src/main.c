#include "world/worldgeneration.h"
#include "world/worldmodel.h"
#include "controller.h"
#include "util.h"

#include <raymath.h>

int main(void)
{
    const int screenWidth = 1920 / 2;
    const int screenHeight = 1080 / 2;

    InitWindow(screenWidth, screenHeight, "Raycraft");

    SetTargetFPS(60);

    int *world = GenerateWorld();
    Model worldModel = GetWorldModel(world);

    Controller controller;
    InitializeController(&controller, world);

    DisableCursor();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(SKYBLUE);

            BeginMode3D(controller.cam);
            {
                UpdateController(&controller);
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

            // DrawText("Made by Michal Pietraszko (twitter: @pietmichal)", 10, 10, 20, WHITE);
                        
            DrawFPS(1920 / 2 - 90, 10);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}