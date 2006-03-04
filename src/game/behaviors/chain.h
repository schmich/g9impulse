#ifndef CHAIN_H
#define CHAIN_H

#include "common.h"
#include "updateable.h"

typedef struct ChainBehavior
{
    BEHAVIOR_BASE;
    Behavior** behaviors;
    uint8 numBehaviors;
} ChainBehavior;

ChainBehavior* createChainBehavior(Behavior** behaviors, uint8 numBehaviors);

#endif // CHAIN_H
