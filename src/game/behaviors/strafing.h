#ifndef STRAFING_H
#define STRAFING_H

#include "common.h"
#include "updateable.h"

typedef struct Strafing
{
    BEHAVIOR_BASE;
    int8  speed;
    uint8 frameDelay;
    uint8 elapsed;
} Strafing;

Strafing* createStrafing(int8 speed, uint8 frameDelay);

#endif // STRAFING_H
