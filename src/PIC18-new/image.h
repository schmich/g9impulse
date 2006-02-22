#ifndef IMAGE_H
#define IMAGE_H

#include "common.h"
#include "gpu_pic.h"

typedef struct Image
{
    uint32 address;
    uint8  width;
    uint8  height;
    bool   transparent;
} Image;

typedef struct Point
{
    int16 x;
    int16 y;
} Point;

Image makeImage(uint32 address, uint8 width, uint8 height);
Image* createImage(uint32 address, uint8 width, uint8 height);

Image makeOpaqueImage(uint32 address, uint8 width, uint8 height);
Image* createOpaqueImage(uint32 address, uint8 width, uint8 height);

void destroyImage(Image* image);

void drawImage(Image* image, Point where, bool isTransparent);

Point makePoint(int16 x, int16 y);

#endif // IMAGE_H
