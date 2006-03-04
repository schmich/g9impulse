#ifndef ENEMY_H
#define ENEMY_H

#include "common.h"
#include "entity.h"

#define ENEMY_BASE ENTITY_BASE

typedef struct Enemy
{
    ENEMY_BASE;
} Enemy;

Enemy* createEnemy(Animation* anim,
                   uint8 initFrame,
                   Behavior* behavior,
                   uint8 health,
                   Point where,
                   SpawnFn onProjectileSpawn,
                   KillFn onKill);

#endif // ENEMY_H
