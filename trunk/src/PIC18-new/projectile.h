#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "common.h"
#include "updateable.h"

struct Projectile;
typedef near rom void (*ImpactFn)(struct Projectile*, Sprite*, World*);

#define PROJECTILE_BASE UPDATEABLE_BASE; \
                        uint8 damage; \
                        ImpactFn impact

typedef struct Projectile
{
    PROJECTILE_BASE;
} Projectile;

Projectile* createProjectile(Animation* anim,
                             uint8 initFrame,
                             Behavior* behavior,
                             uint8 damage,
                             Point where,
                             ImpactFn onImpact);

void impact(Projectile* proj, Sprite* who, World* world);

#endif // PROJECTILE_H
