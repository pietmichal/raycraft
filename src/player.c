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
    player->size = (Vector3){1.0f, 1.2f, 1.0f};
    player->velocity = (Vector3){0};
    player->friction = 10.0f;

    return player;
}

static void HandlePlayerMovement(Player *player)
{
    player->yaw -= GetMouseMovement().x;

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

    player->velocity.x -= GetFrameTime() * player->friction;
    player->velocity.y -= GetFrameTime() * player->friction;
    player->velocity.z -= GetFrameTime() * player->friction;

    // todo: Make velocity dependent on yaw
    player->velocity.x = fmaxf(player->velocity.x, 0);
    player->velocity.y = fmaxf(player->velocity.y, 0);
    player->velocity.z = fmaxf(player->velocity.z, 0);

    player->position.x += player->velocity.x * GetFrameTime();
    player->position.y += player->velocity.y * GetFrameTime();
    player->position.z += player->velocity.z * GetFrameTime();
}

static void HandlePlayerWorldCollision(Player *player, int *world)
{
    Vector3 playerWorldPosition = GetWorldPosition(player->position);
    // todo: get colliding blocks and push out
}

void UpdatePlayer(Player *player, int *world)
{
    HandlePlayerMovement(player);
    HandlePlayerWorldCollision(player, world);
}