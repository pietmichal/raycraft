#include "raylib.h"
#include "stdlib.h"
#include "gamecamera.h"

GameCamera *CreateGameCamera(Vector3 position)
{
    GameCamera *gameCamera = RL_MALLOC(sizeof(GameCamera));

    gameCamera->camera.position = position;
    gameCamera->camera.target = (Vector3){0};
    gameCamera->camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    gameCamera->camera.fovy = 60.0f;
    gameCamera->camera.type = CAMERA_PERSPECTIVE;
    gameCamera->rotation = (Vector3){0};

    return gameCamera;
}

void AttachGameCameraToPlayer(GameCamera *gameCamera, void *player)
{
}