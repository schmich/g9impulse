#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "common.h"
#include "updateable.h"

#define FOREVER 0

typedef struct Animator
{
    BEHAVIOR_BASE;
    uint8 frameDelay;
    uint8 elapsed;
    uint8 loops;
} Animator;

Animator* createAnimator(uint8 frameDelay, uint8 loopCount);

#endif // ANIMATOR_H
