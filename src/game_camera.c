#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "game_camera.h"
#include "player.h"
#include "game_mouse.h"

GameCamera *CreateGameCamera()
{
    GameCamera *gameCamera = RL_MALLOC(sizeof(GameCamera));

    gameCamera->camera.position = (Vector3){0};
    gameCamera->camera.target = (Vector3){0};
    gameCamera->camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    gameCamera->camera.fovy = 60.0f;
    gameCamera->camera.type = CAMERA_PERSPECTIVE;
    gameCamera->rotation = (Vector3){0};

    return gameCamera;
}

void UpdateGameCamera(GameCamera *gameCamera, Player *player)
{
    gameCamera->camera.position.x = player->position.x;
    // Note: place camera on top of player's head.
    gameCamera->camera.position.y = player->position.y + (player->size.y/2);
    gameCamera->camera.position.z = player->position.z;

    gameCamera->rotation.y -= GetMouseMovement().y;

    Matrix translation = MatrixTranslate(0, 0, 10); // Why 10?
    Matrix rotation = MatrixRotateXYZ((Vector3){PI * 2 - gameCamera->rotation.y, PI * 2 - player->yaw, 0});
    Matrix transform = MatrixMultiply(translation, rotation);

    gameCamera->camera.target.x = gameCamera->camera.position.x - transform.m12;
    gameCamera->camera.target.y = gameCamera->camera.position.y - transform.m13;
    gameCamera->camera.target.z = gameCamera->camera.position.z - transform.m14;
}