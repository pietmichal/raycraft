#include "util.h"

float roundHalf(float f)
{
    int full = (int)f;
    return (float)full + 0.5f;
}

Vector3 Vector3RoundDown(Vector3 v)
{
    return (Vector3){(float)(int)v.x,
                     (float)(int)v.y,
                     (float)(int)v.z};
}

Vector3 Vector3RoundHalf(Vector3 v)
{
    return (Vector3){roundHalf(v.x),
                     roundHalf(v.y),
                     roundHalf(v.z)};
}
