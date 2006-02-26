#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "common.h"
#include "updateable.h"

typedef struct Boring
{
    BEHAVIOR_BASE;
    int8 speed;
} Boring;

typedef struct Animator
{
    BEHAVIOR_BASE;
    uint8 frames;
    uint8 elapsed;
} Animator;

typedef struct ChainBehavior
{
    BEHAVIOR_BASE;
    Behavior** behaviors;
    uint8 numBehaviors;
} ChainBehavior;

Boring* createBoring(int8 speed);
Animator* createAnimator(uint8 speed);
ChainBehavior* createChainBehavior(Behavior** behaviors, uint8 numBehaviors);

#endif // BEHAVIOR_H
