#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "rlgl.h"

#define WORLD_MAX_X 220
#define WORLD_MAX_Z 220
#define WORLD_MAX_Y 8

#include <stdio.h>
#include <string.h>

#define MAX_MESH_VBO 7 // Maximum number of vbo per mesh

static int SEED = 0;

static int hash[] = {208, 34, 231, 213, 32, 248, 233, 56, 161, 78, 24, 140, 71, 48, 140, 254, 245, 255, 247, 247, 40,
                     185, 248, 251, 245, 28, 124, 204, 204, 76, 36, 1, 107, 28, 234, 163, 202, 224, 245, 128, 167, 204,
                     9, 92, 217, 54, 239, 174, 173, 102, 193, 189, 190, 121, 100, 108, 167, 44, 43, 77, 180, 204, 8, 81,
                     70, 223, 11, 38, 24, 254, 210, 210, 177, 32, 81, 195, 243, 125, 8, 169, 112, 32, 97, 53, 195, 13,
                     203, 9, 47, 104, 125, 117, 114, 124, 165, 203, 181, 235, 193, 206, 70, 180, 174, 0, 167, 181, 41,
                     164, 30, 116, 127, 198, 245, 146, 87, 224, 149, 206, 57, 4, 192, 210, 65, 210, 129, 240, 178, 105,
                     228, 108, 245, 148, 140, 40, 35, 195, 38, 58, 65, 207, 215, 253, 65, 85, 208, 76, 62, 3, 237, 55, 89,
                     232, 50, 217, 64, 244, 157, 199, 121, 252, 90, 17, 212, 203, 149, 152, 140, 187, 234, 177, 73, 174,
                     193, 100, 192, 143, 97, 53, 145, 135, 19, 103, 13, 90, 135, 151, 199, 91, 239, 247, 33, 39, 145,
                     101, 120, 99, 3, 186, 86, 99, 41, 237, 203, 111, 79, 220, 135, 158, 42, 30, 154, 120, 67, 87, 167,
                     135, 176, 183, 191, 253, 115, 184, 21, 233, 58, 129, 233, 142, 39, 128, 211, 118, 137, 139, 255,
                     114, 20, 218, 113, 154, 27, 127, 246, 250, 1, 8, 198, 250, 209, 92, 222, 173, 21, 88, 102, 219};

int noise2(int x, int y)
{
    int tmp = hash[(y + SEED) % 256];
    return hash[(tmp + x) % 256];
}

float lin_inter(float x, float y, float s)
{
    return x + s * (y - x);
}

float smooth_inter(float x, float y, float s)
{
    return lin_inter(x, y, s * s * (3 - 2 * s));
}

float noise2d(float x, float y)
{
    int x_int = x;
    int y_int = y;
    float x_frac = x - x_int;
    float y_frac = y - y_int;
    int s = noise2(x_int, y_int);
    int t = noise2(x_int + 1, y_int);
    int u = noise2(x_int, y_int + 1);
    int v = noise2(x_int + 1, y_int + 1);
    float low = smooth_inter(s, t, x_frac);
    float high = smooth_inter(u, v, x_frac);
    return smooth_inter(low, high, y_frac);
}

float perlin2d(float x, float y, float freq, int depth)
{
    float xa = x * freq;
    float ya = y * freq;
    float amp = 1.0;
    float fin = 0;
    float div = 0.0;

    int i;
    for (i = 0; i < depth; i++)
    {
        div += 256 * amp;
        fin += noise2d(xa, ya) * amp;
        amp /= 2;
        xa *= 2;
        ya *= 2;
    }

    return fin / div;
}

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