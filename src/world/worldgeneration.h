// raycraft (c) Nikolas Wipper 2020

#ifndef RAYCRAFT_WORLDGENERATION_H
#define RAYCRAFT_WORLDGENERATION_H

#define WORLD_MAX_X 100
#define WORLD_MAX_Z 100
#define WORLD_MAX_Y 16

#include <raylib.h>
#include <raymath.h>

int GetWorldCube(int *world, int x, int y, int z);
void SetWorldCube(int *world, int x, int y, int z, int value);
int *GenerateWorld();

#endif //RAYCRAFT_WORLDGENERATION_H
