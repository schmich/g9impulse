#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "common.h"
#include "updateable.h"

typedef struct Animator
{
    BEHAVIOR_BASE;
    uint8 frames;
    uint8 elapsed;
} Animator;

Animator* createAnimator(uint8 speed);

#endif // ANIMATOR_H
