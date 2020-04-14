#ifndef RAYCRAFT_CONTROLLER_H
#define RAYCRAFT_CONTROLLER_H

#include <raylib.h>

#define FORWARD_KEY KEY_W
#define BACKWARD_KEY KEY_S
#define LEFT_KEY KEY_A
#define RIGHT_KEY KEY_D

#define PLAYER_MOVEMENT_SENSITIVITY 3
#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.01f

typedef struct Controller
{
    struct Vector3 rotation;
    struct Camera3D cam;
    int *world;
    bool onGround;
} Controller;

void InitializeController(Controller *controller, int *world);
void UpdateController(Controller *controller);
Vector3 GetPlayerBlockPos(Controller *controller);

#endif //RAYCRAFT_CONTROLLER_H
