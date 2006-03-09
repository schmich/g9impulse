#ifndef HEATMETER_H
#define HEATMETER_H

#include "common.h"
#include "updateable.h"
#include "player.h"

typedef struct HeatMeter
{
    UPDATEABLE_BASE;
    Player* player;
    uint8   frameDelay;
} HeatMeter;

HeatMeter* createHeatMeter(Player* who, Point where);

#endif // HEATMETER_H
