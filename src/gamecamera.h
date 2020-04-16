#ifndef RAYCRAFT_CAMERA_H
#define RAYCRAFT_CAMERA_H

#include "raylib.h"

typedef struct GameCamera
{
    struct Camera3D camera;
    Vector3 rotation;
} GameCamera;


GameCamera *CreateGameCamera(Vector3 position);
void AttachGameCameraToPlayer(GameCamera *gameCamera, void *player);

#endif