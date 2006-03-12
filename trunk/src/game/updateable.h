#ifndef UPDATEABLE_H
#define UPDATEABLE_H

#include "common.h"
#include "sprite.h"
#include "destroyable.h"
#include "world.h"

#define UPDATE_KEEP    0
#define UPDATE_REMOVE  1

struct Updateable;
typedef near rom uint8 (*UpdateFn)(struct Updateable*, World*);

#define BEHAVIOR_BASE DESTROYABLE_BASE; \
                      UpdateFn update

typedef struct Behavior
{
    BEHAVIOR_BASE;
} Behavior;

#define UPDATEABLE_BASE SPRITE_BASE; \
                        Behavior* behavior

typedef struct Updateable
{
    UPDATEABLE_BASE;
} Updateable;

Behavior* createBehavior(UpdateFn update);
Updateable* createUpdateable(Animation* anim,
                             uint8 initFrame,
                             Behavior* behavior,
                             Point where);

uint8 update(Updateable* who, World* world);

uint8 nullUpdate(Updateable* who, World* world);

#endif // UPDATEABLE_H
