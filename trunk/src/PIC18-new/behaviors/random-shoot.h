#ifndef RANDOMSHOOT_H
#define RANDOMSHOOT_H

#include "common.h"
#include "updateable.h"

typedef struct RandomShoot
{
    BEHAVIOR_BASE;
    uint16 frequency;
} RandomShoot;

RandomShoot* createRandomShoot(uint16 frequency);

#endif // RANDOMSHOOT_H
