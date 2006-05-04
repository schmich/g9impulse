#ifndef STATS_H
#define STATS_H

#include "common.h"
#include "destroyable.h"

typedef struct Stats
{
    DESTROYABLE_BASE;

    uint16 nukesFired;
    uint16 shotsFired;
    uint16 shotsLanded;

    uint16 artifactsGiven;
    uint16 artifactsCollected;

    uint16 shotsFaced;
    uint16 enemiesFaced;

    uint16 kills;
    uint16 score;

    uint16 livesLost;
    uint16 healthLost;

    uint16 kamikazes;
} Stats;

Stats* createStats(void);

#endif // STATS_H
