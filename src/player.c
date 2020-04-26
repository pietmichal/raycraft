#include "player.h"
#include "raylib.h"
#include "stdlib.h"
#include "math.h"
#include "world/worldgeneration.h"
#include "game_mouse.h"
#include "raymath.h"

Player *CreatePlayer(Vector3 position)
{
    Player *player = RL_MALLOC(sizeof(Player));

    player->position = position;
    player->yaw = 0.0f;
    player->size = (Vector3){1.0f, 1.0f, 1.0f};
    player->velocity = (Vector3){0};
    player->friction = 10.0f;

    return player;
}

static void HandlePlayerMovement(Player *player)
{
    player->yaw -= GetMouseMovement().x;
    player->velocity = Vector3Zero();

    if (IsKeyDown(FORWARD_KEY))
    {
        player->velocity.x = 3;
    }

    if (IsKeyDown(BACKWARD_KEY))
    {
        player->velocity.x = -3;
    }

    if (IsKeyDown(RIGHT_KEY))
    {
        player->velocity.z = 3;
    }

    if (IsKeyDown(LEFT_KEY))
    {
        player->velocity.z = -3;
    }

    // player->velocity.y = -3;

    player->position.x -= (player->velocity.x * sinf(player->yaw) - player->velocity.z * cosf(player->yaw)) * GetFrameTime();
    player->position.z -= (player->velocity.x * cosf(player->yaw) + player->velocity.z * sinf(player->yaw)) * GetFrameTime();
    player->position.y += player->velocity.y * GetFrameTime();
}

static void HandlePlayerWorldCollision(Player *player, int *world)
{
    // todo:
}

void UpdatePlayer(Player *player, int *world)
{
    HandlePlayerMovement(player);
    HandlePlayerWorldCollision(player, world);
}