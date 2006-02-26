#ifndef ANIMATION_H
#define ANIMATION_H

#include "common.h"
#include "image.h"
#include "destroyable.h"

typedef struct Animation
{
    DESTROYABLE_BASE;
    Image*  images;
    uint8   numImages;
} Animation;

Animation* createAnimation(void);

#endif // ANIMATION_H
