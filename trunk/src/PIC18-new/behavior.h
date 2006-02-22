#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "common.h"

typedef struct Boring
{
    BEHAVIOR_BASE;
    uint8 speed;
} Boring;

Boring* createBoring(uint8 speed);

#endif // BEHAVIOR_H
