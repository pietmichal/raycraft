#include "player.h"
#include "raylib.h"
#include "stdlib.h"
#include "math.h"

Player *CreatePlayer(Vector3 position)
{
    Player *player = RL_MALLOC(sizeof(Player));

    player->position = position;
    player->rotation = (Vector3){0};
    player->size = (Vector3){1.0f, 1.2f, 1.0f};
    player->velocity = (Vector3){0};
    player->friction = 10.0f;

    return player;
}

void UpdatePlayer(Player *player)
{

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

    player->velocity.x = fmaxf(player->velocity.x, 0);
    player->velocity.y = fmaxf(player->velocity.y, 0);
    player->velocity.z = fmaxf(player->velocity.z, 0);

    player->position.x += player->velocity.x * GetFrameTime();
    player->position.y += player->velocity.y * GetFrameTime();
    player->position.z += player->velocity.z * GetFrameTime();
}