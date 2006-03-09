#ifndef ROLL_H
#define ROLL_H

#include "common.h"
#include "updateable.h"

typedef struct Roll
{
    BEHAVIOR_BASE;
    int16 xPrevious;
} Roll;

Roll* createRoll(int16 xInit);

#endif // ROLL_H
