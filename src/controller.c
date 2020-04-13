// raycraft (c) Nikolas Wipper 2020

#include "controller.h"

#include <raymath.h>
#include <stdio.h>

#include "world/worldgeneration.h"
#include "util.h"

static Vector2 lastMousePos;
static Vector3 movement = {0, -0.2f, 0};

Vector3 getForceToDirection(Vector3 direction, Vector3 currentMotion)
{
    Vector3 revertedMotion = {0,0,0};

#define CALC_AXIS(a) if ((currentMotion. a > 0 && direction. a < 0) || (currentMotion. a < 0 && direction. a > 0)) \
                        revertedMotion. a = direction. a * (direction. a * currentMotion. a )
    CALC_AXIS(x);
    CALC_AXIS(y);
    CALC_AXIS(z);

    return revertedMotion;
}

bool isColliderInDirection(Vector3 position, Vector3 direction, int *world)
{
    return GetWorldCubeV(world, Vector3Add(position, direction)) ||
           GetWorldCubeV(world, Vector3Add(position, Vector3Add(direction, (Vector3){ 0,-1, 0})));
}

void InitializeController(Controller *controller, int *world)
{
    controller->cam.position = (Vector3){1.0f, 10.0f, 1.0f};
    controller->cam.target = (Vector3){5.0f, 10.0f, 5.0f};
    controller->cam.up = (Vector3){0.0f, 1.0f, 0.0f};
    controller->cam.fovy = 60.0f;
    controller->cam.type = CAMERA_PERSPECTIVE;
    controller->rotation = (Vector3){0.0f, 0.0f, 0.0f};
    controller->world = world;
    lastMousePos = GetMousePosition();
}

void UpdateController(Controller *controller)
{
    // -------------------- Rotation stuff --------------------
    Vector2 mouseMovement = Vector2Subtract(GetMousePosition(), lastMousePos);

    lastMousePos = GetMousePosition();

    controller->rotation.x += (mouseMovement.x*-CAMERA_MOUSE_MOVE_SENSITIVITY);
    controller->rotation.y += (mouseMovement.y*-CAMERA_MOUSE_MOVE_SENSITIVITY);

    //controller->rotation.x = Clamp(controller->rotation.x, -40, 40);

    // -------------------- Movement stuff --------------------

    float direction[] = { IsKeyDown(FORWARD_KEY),
                          IsKeyDown(BACKWARD_KEY),
                          IsKeyDown(RIGHT_KEY),
                          IsKeyDown(LEFT_KEY)};

    movement = (Vector3){0, movement.y + GetFrameTime() * -0.5, 0};
    movement.x = (sinf(controller->rotation.x)*direction[1] -
                  sinf(controller->rotation.x)*direction[0] -
                  cosf(controller->rotation.x)*direction[3] +
                  cosf(controller->rotation.x)*direction[2])/PLAYER_MOVEMENT_SENSITIVITY;

    movement.z = (cosf(controller->rotation.x)*direction[1] -
                  cosf(controller->rotation.x)*direction[0] +
                  sinf(controller->rotation.x)*direction[3] -
                  sinf(controller->rotation.x)*direction[2])/PLAYER_MOVEMENT_SENSITIVITY;

    if (IsKeyPressed(KEY_SPACE))
        movement.y = 0.2f;

    // -------------------- Collision stuff --------------------

    bool blockInFront = isColliderInDirection(GetPlayerBlockPos(controller), (Vector3){ 1, 0, 0}, controller->world);
    bool blockInBack  = isColliderInDirection(GetPlayerBlockPos(controller), (Vector3){-1, 0, 0}, controller->world);
    bool blockInRight = isColliderInDirection(GetPlayerBlockPos(controller), (Vector3){ 0, 0, 1}, controller->world);
    bool blockInLeft  = isColliderInDirection(GetPlayerBlockPos(controller), (Vector3){ 0, 0,-1}, controller->world);

    BoundingBox boundingBoxPlayer = {(Vector3){(controller->cam.position.x - 0.5f),
                                               (controller->cam.position.y - 2.5f),
                                               (controller->cam.position.z - 0.5f)},
                                     (Vector3){(controller->cam.position.x + 0.5f),
                                               (controller->cam.position.y + 1.5f),
                                               (controller->cam.position.z + 0.5f)}};

    BoundingBox boundingBoxPlayerBlock = {(Vector3){(GetPlayerBlockPos(controller).x - 0.5f),
                                                    (GetPlayerBlockPos(controller).y - 1),
                                                    (GetPlayerBlockPos(controller).z - 0.5f)},
                                          (Vector3){(GetPlayerBlockPos(controller).x + 0.5f),
                                                    (GetPlayerBlockPos(controller).y + 1),
                                                    (GetPlayerBlockPos(controller).z + 0.5f)}};

    BoundingBox boundingBoxFront = {Vector3Add(boundingBoxPlayerBlock.min, (Vector3){ 1, 0, 0}),
                                    Vector3Add(boundingBoxPlayerBlock.max, (Vector3){ 1, 0, 0})};
    BoundingBox boundingBoxBack  = {Vector3Add(boundingBoxPlayerBlock.min, (Vector3){-1, 0, 0}),
                                    Vector3Add(boundingBoxPlayerBlock.max, (Vector3){-1, 0, 0})};
    // ------------------------------------------------------------------------------------------
    BoundingBox boundingBoxRight = {Vector3Add(boundingBoxPlayerBlock.min, (Vector3){ 0, 0, 1}),
                                    Vector3Add(boundingBoxPlayerBlock.max, (Vector3){ 0, 0, 1})};
    BoundingBox boundingBoxLeft  = {Vector3Add(boundingBoxPlayerBlock.min, (Vector3){ 0, 0,-1}),
                                    Vector3Add(boundingBoxPlayerBlock.max, (Vector3){ 0, 0,-1})};

    DrawCubeWires(Vector3Subtract(GetPlayerBlockPos(controller), (Vector3){0, 1, 0}),
                  1, -1, 1, RED);

    if (blockInFront && CheckCollisionBoxes(boundingBoxPlayer, boundingBoxFront))
        movement = Vector3Subtract(movement, getForceToDirection((Vector3){-1, 0, 0}, movement));

    if (blockInBack && CheckCollisionBoxes(boundingBoxPlayer, boundingBoxBack))
        movement = Vector3Subtract(movement, getForceToDirection((Vector3){ 1, 0, 0}, movement));

    if (blockInRight && CheckCollisionBoxes(boundingBoxPlayer, boundingBoxRight))
        movement = Vector3Subtract(movement, getForceToDirection((Vector3){ 0, 0,-1}, movement));

    if (blockInLeft && CheckCollisionBoxes(boundingBoxPlayer, boundingBoxLeft))
        movement = Vector3Subtract(movement, getForceToDirection((Vector3){ 0, 0, 1}, movement));

    if (GetWorldCubeV(controller->world, Vector3Add(GetPlayerBlockPos(controller), (Vector3){0,-2, 0})) == 1)
        movement = Vector3Subtract(movement, getForceToDirection((Vector3){ 0, 1, 0}, movement));


    controller->cam.position.x += movement.x / PLAYER_MOVEMENT_SENSITIVITY;

    controller->cam.position.y += movement.y;

    controller->cam.position.z += movement.z / PLAYER_MOVEMENT_SENSITIVITY;

    // -------------------- Retarget stuff --------------------

    Matrix translation = MatrixTranslate(0, 0, (10));
    Matrix rotation = MatrixRotateXYZ((Vector3){ PI*2 - controller->rotation.y, PI*2 - controller->rotation.x, 0 });
    Matrix transform = MatrixMultiply(translation, rotation);

    controller->cam.target.x = controller->cam.position.x - transform.m12;
    controller->cam.target.y = controller->cam.position.y - transform.m13;
    controller->cam.target.z = controller->cam.position.z - transform.m14;
}

Vector3 GetPlayerBlockPos(Controller *controller)
{
    return (Vector3){roundf(controller->cam.position.x),
                     roundf(controller->cam.position.y - 1),
                     roundf(controller->cam.position.z)};
}
