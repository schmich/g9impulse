#ifndef LEVEL_H
#define LEVEL_H

#include "common.h"
#include "background.h"
#include "world.h"

typedef near rom void (*SpawnFn)(World*, uint16);

typedef struct Level
{
    Background*      background;
    SpawnFn          spawn;
    uint16           currentEvent;
    const rom uint16 spawnTimes[100];
} Level;

void updateLevel(Level* level, World* world);

#endif // LEVEL_H
