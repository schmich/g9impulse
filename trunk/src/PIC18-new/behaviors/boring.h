#ifndef BORING_H
#define BORING_H

#include "common.h"
#include "updateable.h"

typedef struct Boring
{
    BEHAVIOR_BASE;
    int8 vertSpeed;
} Boring;

Boring* createBoring(int8 vertSpeed);

#endif // BORING_H
