#ifndef FACING_H
#define FACING_H

#include "common.h"
#include "updateable.h"

typedef struct Facing
{
    BEHAVIOR_BASE;
} Facing;

Facing* createFacing(void);

#endif // FACING_H
