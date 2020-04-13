// raycraft (c) Nikolas Wipper 2020

#ifndef RAYCRAFT_PERLIN_H
#define RAYCRAFT_PERLIN_H

int noise2(int x, int y);
float lin_inter(float x, float y, float s);
float smooth_inter(float x, float y, float s);
float noise2d(float x, float y);
float perlin2d(float x, float y, float freq, int depth);

#endif //RAYCRAFT_PERLIN_H
