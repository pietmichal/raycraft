#ifndef RAYCRAFT_PLAYER_H
#define RAYCRAFT_PLAYER_H

#define FORWARD_KEY KEY_W
#define BACKWARD_KEY KEY_S
#define LEFT_KEY KEY_A
#define RIGHT_KEY KEY_D

#include "raylib.h"

typedef struct Player {
    Vector3 position;
    Vector3 size;
    Vector3 rotation;
    Vector3 velocity;
    float friction; // Note: keep it in the player for simplicity's sake
} Player;

Player *CreatePlayer(Vector3 position);
void UpdatePlayer(Player* player);

#endif