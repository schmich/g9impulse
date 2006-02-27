#ifndef KILLABLE_H
#define KILLABLE_H

#include "common.h"
#include "updateable.h"

struct Killable;
typedef near rom void (*KillFn)(struct Killable*, World*);

#define KILLABLE_BASE UPDATEABLE_BASE; \
                      uint8 health; \
                      KillFn kill

typedef struct Killable
{
    KILLABLE_BASE;
} Killable;

void kill(Killable* who, World* world);
bool damage(Killable* who, uint8 damage);
bool dead(Killable* who);

#endif // KILLABLE_H
