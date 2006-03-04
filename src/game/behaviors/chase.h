#ifndef CHASE_H
#define CHASE_H

#include "common.h"
#include "updateable.h"

typedef struct Chase
{
    BEHAVIOR_BASE;
    int8 vertSpeed;
    int8 horizSpeed;
} Chase;

Chase* createChase(int8 vertSpeed, int8 horizSpeed);

#endif // CHASE_H
