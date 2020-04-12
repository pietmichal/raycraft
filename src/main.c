#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "rlgl.h"

#include "noise.c";

#define WORLD_MAX_X 220
#define WORLD_MAX_Z 220
#define WORLD_MAX_Y 8

#include <stdio.h>
#include <string.h>

#define MAX_MESH_VBO 7 // Maximum number of vbo per mesh


float *GetCubeVertices(float x, float y, float z)
{
    // not using indices
    float width = 1.0f;
    float height = 1.0f;
    float length = 1.0f;
    float *cubeVertices = malloc(36 * 3 * sizeof(float));
    float ref[] = {
        // face 1
        -width / 2 + x, -height / 2 + y, length / 2 + z,
        width / 2 + x, -height / 2 + y, length / 2 + z,
        width / 2 + x, height / 2 + y, length / 2 + z,

        width / 2 + x, height / 2 + y, length / 2 + z,
        -width / 2 + x, height / 2 + y, length / 2 + z,
        -width / 2 + x, -height / 2 + y, length / 2 + z,

        // face 2
        -width / 2 + x, -height / 2 + y, -length / 2 + z,
        -width / 2 + x, height / 2 + y, -length / 2 + z,
        width / 2 + x, height / 2 + y, -length / 2 + z,

        width / 2 + x, height / 2 + y, -length / 2 + z,
        width / 2 + x, -height / 2 + y, -length / 2 + z,
        -width / 2 + x, -height / 2 + y, -length / 2 + z,

        // face 3
        -width / 2 + x, height / 2 + y, -length / 2 + z,
        -width / 2 + x, height / 2 + y, length / 2 + z,
        width / 2 + x, height / 2 + y, length / 2 + z,

        width / 2 + x, height / 2 + y, length / 2 + z,
        width / 2 + x, height / 2 + y, -length / 2 + z,
        -width / 2 + x, height / 2 + y, -length / 2 + z,

        // face 4
        -width / 2 + x, -height / 2 + y, -length / 2 + z,
        width / 2 + x, -height / 2 + y, -length / 2 + z,
        width / 2 + x, -height / 2 + y, length / 2 + z,

        width / 2 + x, -height / 2 + y, length / 2 + z,
        -width / 2 + x, -height / 2 + y, length / 2 + z,
        -width / 2 + x, -height / 2 + y, -length / 2 + z,

        // face 5
        width / 2 + x, -height / 2 + y, -length / 2 + z,
        width / 2 + x, height / 2 + y, -length / 2 + z,
        width / 2 + x, height / 2 + y, length / 2 + z,

        width / 2 + x, height / 2 + y, length / 2 + z,
        width / 2 + x, -height / 2 + y, length / 2 + z,
        width / 2 + x, -height / 2 + y, -length / 2 + z,

        // face 6
        -width / 2 + x, -height / 2 + y, -length / 2 + z,
        -width / 2 + x, -height / 2 + y, length / 2 + z,
        -width / 2 + x, height / 2 + y, length / 2 + z,

        -width / 2 + x, height / 2 + y, length / 2 + z,
        -width / 2 + x, height / 2 + y, -length / 2 + z,
        -width / 2 + x, -height / 2 + y, -length / 2 + z};
    for (int i = 0; i < 36 * 3; i++)
    {
        cubeVertices[i] = ref[i];
    }
    return cubeVertices;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920 / 2;
    const int screenHeight = 1080 / 2;

    InitWindow(screenWidth, screenHeight, "Raycraft");

    Texture2D texture = LoadTexture("resources/grass.png");
    Model cubeModel = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));

    cubeModel.materials[0].maps[MAP_DIFFUSE].texture = texture;

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = {0};
    camera.position = (Vector3){4.0f, WORLD_MAX_Y, 4.0f};
    camera.target = (Vector3){0.0f, 1.8f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.type = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_FIRST_PERSON); // Set a first person camera mode

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    float noise[WORLD_MAX_X][WORLD_MAX_Z] = {};
    for (int x = 0; x < WORLD_MAX_X; x++)
    {
        for (int z = 0; z < WORLD_MAX_Z; z++)
        {
            noise[x][z] = perlin2d(x, z, 0.1, WORLD_MAX_Y);
        }
    }

    int world[WORLD_MAX_X][WORLD_MAX_Z][WORLD_MAX_Y]; // uses world max value

    int worldBlockCount = 0;
    for (int x = 0; x < WORLD_MAX_X; x++)
    {
        for (int z = 0; z < WORLD_MAX_Z; z++)
        {
            for (int y = 0; y < WORLD_MAX_Y; y++)
            {
                int maxY = round(WORLD_MAX_Y * noise[x][z]);
                if (y <= maxY)
                {
                    world[x][z][y] = 1;
                    worldBlockCount++;
                }
                else
                {
                    world[x][z][y] = 0;
                }
            }
        }
    }

    // --- GEN WORLD MESH

    Mesh mesh = {0};
    mesh.vboId = (unsigned int *)RL_CALLOC(MAX_MESH_VBO, sizeof(unsigned int));

    float texcoordsRef[] = {
        // face 1
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        // face 2
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        // face 3
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // face 4
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        // face 5
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        // face 6
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f};

    float normalsRef[] = {
        // face 1
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // face 2
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        // face 3
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // face 4
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        // face 5
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // face 6
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f};

    float *vertices = malloc(36 * 3 * WORLD_MAX_X * WORLD_MAX_Y * WORLD_MAX_Z * sizeof(float));
    float *texcoords = malloc(36 * 2 * WORLD_MAX_X * WORLD_MAX_Y * WORLD_MAX_Z * sizeof(float));
    float *normals = malloc(36 * 3 * WORLD_MAX_X * WORLD_MAX_Y * WORLD_MAX_Z * sizeof(float));
    int verticesCount = 0;
    int texcoordsCount = 0;
    int normalsCount = 0;
    for (int x = 0; x < WORLD_MAX_X; x++)
    {
        for (int z = 0; z < WORLD_MAX_Z; z++)
        {
            for (int y = 0; y < WORLD_MAX_Y; y++)
            {
                bool blockExists = world[x][z][y] == 1;
                if (blockExists)
                {
                    float *blockVertices = GetCubeVertices(x, y, z);
                    for (int v = 0; v < 36 * 3; v++)
                    {
                        vertices[verticesCount + v] = blockVertices[v];
                    }
                    for (int t = 0; t < 36 * 2; t++)
                    {
                        texcoords[texcoordsCount + t] = texcoordsRef[t];
                    }
                    for (int n = 0; n < 36 * 3; n++)
                    {
                        normals[normalsCount + n] = normalsRef[n];
                    }
                    verticesCount += 36 * 3;
                    texcoordsCount += 36 * 2;
                    normalsCount += 36 * 3;
                }
            }
        }
    }

    mesh.vertices = (float *)RL_MALLOC(verticesCount * sizeof(float));
    memcpy(mesh.vertices, vertices, verticesCount * sizeof(float));

    mesh.texcoords = (float *)RL_MALLOC(texcoordsCount * sizeof(float));
    memcpy(mesh.texcoords, texcoords, texcoordsCount * sizeof(float));

    mesh.normals = (float *)RL_MALLOC(normalsCount * sizeof(float));
    memcpy(mesh.normals, normals, normalsCount * sizeof(float));

    mesh.vertexCount = verticesCount / 3; // Why divide by 3 ???
    mesh.triangleCount = (verticesCount / 3) / 2;

    rlLoadMesh(&mesh, false);

    Model worldModel = LoadModelFromMesh(mesh);

    worldModel.materials[0].maps[MAP_DIFFUSE].texture = texture;

    // --- END OF WORLD MESH GEN

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        UpdateCamera(&camera); // Update camera

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(SKYBLUE);

        BeginMode3D(camera);

        // Clouds
        DrawCube((Vector3){300.0f, 200.0f, 100.0f}, 100.0f, 10.0f, 37.0f, WHITE);
        DrawCube((Vector3){250.0f, 200.0f, 150.0f}, 49.0f, 10.0f, 40.0f, WHITE);
        DrawCube((Vector3){0.0f, 200.0f, 47.0f}, 60.0f, 10.0f, 300.0f, WHITE);
        DrawCube((Vector3){200.0f, 200.0f, -50.0f}, 30.0f, 10.0f, 37.0f, WHITE);
        DrawCube((Vector3){1000.0f, 200.0f, 200.0f}, 30.0f, 10.0f, 37.0f, WHITE);
        DrawCube((Vector3){50.0f, 200.0f, 30.0f}, 30.0f, 10.0f, 37.0f, WHITE);
        // End of clouds

        DrawModel(worldModel, (Vector3){0, 0, 0}, 1.0f, WHITE);

        EndMode3D();

        DrawText("Raycraft\nMade by Michal (twitter: @pietmichal)\nPowered by raylib.com", 10, 10, 20, WHITE);
        DrawFPS(1920 / 2 - 90, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}