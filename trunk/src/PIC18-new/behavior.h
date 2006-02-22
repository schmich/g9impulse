#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "common.h"
#include "updateable.h"

typedef struct Boring
{
    BEHAVIOR_BASE;
    int8 speed;
} Boring;

Boring* createBoring(int8 speed);

#endif // BEHAVIOR_H
