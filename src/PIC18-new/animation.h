#ifndef ANIMATION_H
#define ANIMATION_H

#include "common.h"
#include "image.h"

typedef struct Animation
{
    Image*  images;
    uint8   numImages;
} Animation;

#endif // ANIMATION_H
