#ifndef SPAWNER_H
#define SPAWNER_H

#include "common.h"
#include "updateable.h"

#define SPAWNER_UPDATEABLE        0
#define SPAWNER_ENEMY             1
#define SPAWNER_ENEMY_PROJECTILE  2
#define SPAWNER_PLAYER_PROJECTILE 3
#define SPAWNER_ARTIFACT          4

typedef struct Spawner
{
    UPDATEABLE_BASE;
    uint8       delay;
    uint8       type;
    Updateable* spawnee;
} Spawner;

Spawner* createSpawner(Updateable* who, uint8 delay, uint8 type);

#endif // SPAWNER_H
