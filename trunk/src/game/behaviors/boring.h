#ifndef BORING_H
#define BORING_H

#include "common.h"
#include "updateable.h"

typedef struct Boring
{
    BEHAVIOR_BASE;
    int8  speed;
    uint8 frameDelay;
    uint8 elapsed;
} Boring;

Boring* createBoring(int8 speed, uint8 frameDelay);

#endif // BORING_H
