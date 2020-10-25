#include "player.h"
#include "raylib.h"
#include "stdlib.h"
#include "math.h"
#include "world/worldgeneration.h"
#include "game_mouse.h"
#include "raymath.h"
#include <assert.h>

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

    player->velocity.y = -3;
}

static void HandlePlayerWorldCollision(Player *player, int *world)
{
    // Get player position

    float x = player->position.x;
    float y = player->position.y;
    float z = player->position.z;
    Vector3 size = player->size;

    // Translate player position to world position
    // Assumption: cube size is always 1.0f!

    Vector3 playerWorldPosition = (Vector3){round(x), round(y), round(z)};

    DrawBoundingBox((BoundingBox){
                        (Vector3){playerWorldPosition.x - 0.5f, playerWorldPosition.y - 0.5f, playerWorldPosition.z - 0.5f},
                        (Vector3){playerWorldPosition.x + 0.5f, playerWorldPosition.y + 0.5f, playerWorldPosition.z + 0.5f},
                    },
                    GREEN);

    // Get cubes around player

    BoundingBox cubes[6 * 6 * 6];

    int iter = 0;

    for (int x = -2; x <= 3; x++)
    {
        for (int y = -2; y <= 3; y++)
        {
            for (int z = -2; z <= 3; z++)
            {
                int cubeX = (int)playerWorldPosition.x + x;
                int cubeY = (int)playerWorldPosition.y + y;
                int cubeZ = (int)playerWorldPosition.z + z;
                int worldCube = GetWorldCube(world, cubeX, cubeY, cubeZ);
                if (worldCube == 1)
                {
                    cubes[iter] = (BoundingBox){
                        (Vector3){cubeX - 0.5f, cubeY - 0.5f, cubeZ - 0.5f},
                        (Vector3){cubeX + 0.5f, cubeY + 0.5f, cubeZ + 0.5f},
                    };
                }
                else
                {
                    // fixme: How do I express that cube doesn't exist?
                    cubes[iter] = (BoundingBox){
                        (Vector3){0, 0, 0},
                        (Vector3){1, 1, 1},
                    };
                }
                iter++;
            }
        }
    }

    for (int i = 0; i < 6 * 6 * 6; i++)
    {
        DrawBoundingBox(cubes[i], BLUE);
    }

    // Naive: move, if collides, rollback

    float newX = player->position.x - ((player->velocity.x * sinf(player->yaw) - player->velocity.z * cosf(player->yaw)) * GetFrameTime());
    float newY = player->position.y + (player->velocity.y * GetFrameTime());
    float newZ = player->position.z - ((player->velocity.x * cosf(player->yaw) + player->velocity.z * sinf(player->yaw)) * GetFrameTime());

    player->position.x = newX;
    player->position.z = newZ;

    // Translate new player position and size to box

    BoundingBox playerBox = (BoundingBox){
        (Vector3){newX - size.x / 2, newY - size.y / 2, newZ - size.z / 2},
        (Vector3){newX + size.x / 2, newY + size.y / 2, newZ + size.z / 2}};

    bool isColliding = false;
    for (int i = 0; i < 6 * 6 * 6; i++)
    {
        // fixme: detect that cube doesn't exist
        isColliding = CheckCollisionBoxes(playerBox, cubes[i]);
        if (isColliding)
        {
            DrawBoundingBox(cubes[i], PINK);
            break;
        }
    }

    if (!isColliding)
    {
        player->position.y = newY;
    }

    // todo: one axis move, if it collides, rollback, repeat until out of axis

    // Even less naive: instead of rolling back, move to the intersection point
}

void UpdatePlayer(Player *player, int *world)
{
    HandlePlayerMovement(player);
    HandlePlayerWorldCollision(player, world);
}