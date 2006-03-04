#ifndef ENEMY_H
#define ENEMY_H

#include "common.h"
#include "killable.h"
#include "projectile.h"

#define ENEMY_BASE KILLABLE_BASE; \
                   Projectile* weapon

typedef struct Enemy
{
    ENEMY_BASE;
} Enemy;

Enemy* createEnemy(Animation* anim,
                   uint8 initFrame,
                   Behavior* behavior,
                   uint8 health,
                   Point where,
                   Projectile* weapon,
                   KillFn onKill);

void fire(Enemy* e, World* w);

#endif // ENEMY_H
