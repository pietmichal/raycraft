#ifndef RAYCRAFT_CAMERA_H
#define RAYCRAFT_CAMERA_H

#include "raylib.h"
#include "player.h"

typedef struct GameCamera
{
    struct Camera3D camera;
    Vector3 rotation;
} GameCamera;


GameCamera *CreateGameCamera();
void UpdateGameCamera(GameCamera *gameCamera, Player *player);

#endif