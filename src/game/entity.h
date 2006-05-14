#ifndef ENTITY_H
#define ENTITY_H

#include "common.h"
#include "updateable.h"
#include "projectile.h"

#define MAX_HEALTH 65535

typedef struct Entity Entity;
typedef near rom void (*KillFn)(Entity*, World*);
typedef near rom void (*FireFn)(Entity*, World*);
typedef near rom void (*SpawnFn)(Entity*, World*, Point);

#define ENTITY_BASE UPDATEABLE_BASE; \
                    uint16  health; \
                    KillFn  kill; \
                    FireFn  fire; \
                    SpawnFn spawnProjectile

typedef struct Entity
{
    ENTITY_BASE;
} Entity;

void kill(Entity* who, World* world);
bool damage(Entity* who, uint16 damage);
bool dead(Entity* who);
void fire(Entity* e, World* w);

void nullFire(Entity* e, World* w);
void nullProjectileSpawn(Entity* e, World* w, Projectile** p);

#endif // ENTITY_H
