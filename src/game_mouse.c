#include "raylib.h"
#include "raymath.h"
#include "game_mouse.h"

#define SMOOTHNESS 0.001f
#define SENSITIVITY 2.0f

// Note: Can we get away without using globals?
static Vector2 previousMousePosition = (Vector2){0};
static Vector2 mouseMovement = (Vector2){0};

// Call ONCE at the beginning of every frame!
void UpdateMouseMovement()
{
    Vector2 currentMousePosition = GetMousePosition();
    mouseMovement = Vector2MultiplyV(
        Vector2Subtract(currentMousePosition, previousMousePosition),
        (Vector2){SMOOTHNESS * SENSITIVITY, SMOOTHNESS * SENSITIVITY});
    previousMousePosition = currentMousePosition;
}

Vector2 GetMouseMovement()
{
    return mouseMovement;
}