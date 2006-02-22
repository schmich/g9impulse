#ifndef UPDATEABLE_H
#define UPDATEABLE_H

#include "common.h"
#include "entity.h"
#include "destroyable.h"

struct Updateable;
typedef far rom void (*UpdateFn)(struct Updateable*);

#define BEHAVIOR_BASE \
    DESTROYABLE_BASE; \
    UpdateFn update;

typedef struct Behavior
{
    BEHAVIOR_BASE;
} Behavior;

#define UPDATEABLE_BASE \
    ENTITY_BASE; \
    Behavior* behavior;

typedef struct Updateable
{
    UPDATEABLE_BASE;
} Updateable;

void update(Updateable* who);
Behavior* createBehavior(UpdateFn update);

#endif // UPDATEABLE_H
