#ifndef IMAGE_H
#define IMAGE_H

#include "common.h"
#include "point.h"

typedef struct Image
{
    uint32 address;
    uint16 width;
    uint16 height;
    bool   transparent;
} Image;

Image makeImage(uint32 address, uint16 width, uint16 height);
Image* createImage(uint32 address, uint16 width, uint16 height);

Image makeOpaqueImage(uint32 address, uint16 width, uint16 height);
Image* createOpaqueImage(uint32 address, uint16 width, uint16 height);

void drawImage(rom Image* image, Point where, bool isTransparent);

#endif // IMAGE_H
