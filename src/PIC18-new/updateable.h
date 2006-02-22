#ifndef UPDATEABLE_H
#define UPDATEABLE_H

#include "common.h"
#include "entity.h"
#include "destroyable.h"
#include "world.h"

struct Updateable;
typedef near rom void (*UpdateFn)(struct Updateable*, World*);

#define BEHAVIOR_BASE DESTROYABLE_BASE; \
                      UpdateFn update

typedef struct Behavior
{
    BEHAVIOR_BASE;
} Behavior;

#define UPDATEABLE_BASE ENTITY_BASE; \
                        Behavior* behavior

typedef struct Updateable
{
    UPDATEABLE_BASE;
} Updateable;

void update(Updateable* who, World* world);
Behavior* createBehavior(UpdateFn update);

#endif // UPDATEABLE_H
