#ifndef HEALTHMETER_H
#define HEALTHMETER_H

#include "common.h"
#include "updateable.h"
#include "player.h"

typedef struct HealthMeter
{
    UPDATEABLE_BASE;
    Player* player;
    uint8   frameDelay;
} HealthMeter;

HealthMeter* createHealthMeter(Player* who, Point where);

#endif // HEALTHMETER_H
