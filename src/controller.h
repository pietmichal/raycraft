// raycraft (c) Nikolas Wipper 2020

#ifndef RAYCRAFT_CONTROLLER_H
#define RAYCRAFT_CONTROLLER_H

#include <raylib.h>

#define FORWARD_KEY KEY_W
#define BACKWARD_KEY KEY_S
#define LEFT_KEY KEY_A
#define RIGHT_KEY KEY_D

#define PLAYER_MOVEMENT_SENSITIVITY 3
#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.01f

struct Controller
{
    struct Vector3 rotation;
    struct Camera3D cam;
    int *world;
};

void InitializeController(struct Controller *controller, int *world);
void UpdateController(struct Controller *controller);
Vector3 GetPlayerBlockPos(struct Controller *controller);

#endif //RAYCRAFT_CONTROLLER_H
