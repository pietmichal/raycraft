#ifndef _WORLDGENERATION_C_
#define _WORLDGENERATION_C_

#include "worldgeneration.h"

#include "perlin.h"

#include <stdlib.h>
#include <raymath.h>
#include <stdlib.h>

// todo: decide if (x,y,z) or (x,z,y)
// right now I'm just making cryptic assumptions in the code

int GetWorldCube(int *world, int x, int y, int z)
{
    if (x < WORLD_MAX_X && x >= 0 &&
        y < WORLD_MAX_Y && y >= 0 &&
        z < WORLD_MAX_Z && z >= 0)
        return world[(y * WORLD_MAX_X * WORLD_MAX_Z) + (z * WORLD_MAX_X) + x];
    return 0;
}

void SetWorldCube(int *world, int x, int y, int z, int value)
{
    if (x < WORLD_MAX_X && x >= 0 &&
        y < WORLD_MAX_Y && y >= 0 &&
        z < WORLD_MAX_Z && z >= 0)
        world[(y * WORLD_MAX_X * WORLD_MAX_Z) + (z * WORLD_MAX_X) + x] = value;
}

int *GenerateWorld()
{
    int *world = RL_MALLOC(WORLD_MAX_X * WORLD_MAX_Y * WORLD_MAX_Z * sizeof(int));

    for (int x = 0; x < WORLD_MAX_X; x++)
    {
        for (int y = 0; y < WORLD_MAX_Y; y++)
        {
            for (int z = 0; z < WORLD_MAX_Z; z++)
            {
                int maxY = round(WORLD_MAX_Y * perlin2d(x, z, 0.1f, WORLD_MAX_Y));
                if (y <= maxY)
                {
                    SetWorldCube(world, x, y, z, rand() % 2 + 1);
                }
                else
                {
                    SetWorldCube(world, x, y, z, 0);
                }
            }
        }
    }

    return world;
}

int GetWorldCubeV(int *world, Vector3 pos)
{
    return GetWorldCube(world, (int)pos.x, (int)pos.y, (int)pos.z);
}

#endif