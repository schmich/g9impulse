#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "common.h"
#include "updateable.h"

struct Projectile;
typedef near rom void (*ImpactFn)(struct Projectile*, Entity*, World*);

#define PROJECTILE_BASE UPDATEABLE_BASE; \
                        uint8 damage; \
                        ImpactFn impact

typedef struct Projectile
{
    PROJECTILE_BASE;
} Projectile;

void impact(Projectile* proj, Entity* who, World* world);

#endif // PROJECTILE_H
