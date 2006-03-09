#ifndef ENTITY_H
#define ENTITY_H

#include "common.h"
#include "updateable.h"
#include "projectile.h"

struct Entity;
typedef near rom void (*KillFn)(struct Entity*, World*);
typedef near rom void (*FireFn)(struct Entity*, World*);
typedef near rom void (*SpawnFn)(struct Entity*, World*, Projectile** p);

#define ENTITY_BASE UPDATEABLE_BASE; \
                    uint8   health; \
                    KillFn  kill; \
                    FireFn  fire; \
                    SpawnFn spawnProjectile

typedef struct Entity
{
    ENTITY_BASE;
} Entity;

void kill(Entity* who, World* world);
bool damage(Entity* who, uint8 damage);
bool dead(Entity* who);
void fire(Entity* e, World* w);

#endif // ENTITY_H
