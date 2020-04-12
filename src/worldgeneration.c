#ifndef _WORLDGENERATION_C_
#define _WORLDGENERATION_C_

#include "perlin.c"

#define WORLD_MAX_X 220
#define WORLD_MAX_Z 220
#define WORLD_MAX_Y 10

// todo: decide if (x,y,z) or (x,z,y)
// right now I'm just making cryptic assumptions in the code

int GetWorldCube(int *world, int x, int z, int y)
{
    return world[(z * WORLD_MAX_X * WORLD_MAX_Y) + (y * WORLD_MAX_X) + x];
}

void SetWorldCube(int *world, int x, int z, int y, int value)
{
    world[(z * WORLD_MAX_X * WORLD_MAX_Y) + (y * WORLD_MAX_X) + x] = value;
}

int *GenerateWorld()
{
    int *world = RL_MALLOC(WORLD_MAX_X * WORLD_MAX_Y * WORLD_MAX_Z * sizeof(int));

    for (int x = 0; x < WORLD_MAX_X; x++)
    {
        for (int z = 0; z < WORLD_MAX_Z; z++)
        {
            for (int y = 0; y < WORLD_MAX_Y; y++)
            {
                int maxY = round(WORLD_MAX_Y * perlin2d(x, z, 0.1, WORLD_MAX_Y));
                if (y <= maxY)
                {
                    SetWorldCube(world, x, z, y, 1);
                }
                else
                {
                    SetWorldCube(world, x, z, y, 0);
                }
            }
        }
    }

    return world;
}

#endif