#ifndef DESTROYABLE_H
#define DESTROYABLE_H

#include "common.h"

struct Destroyable;
typedef near rom void (*DestroyFn)(struct Destroyable*);

#define DESTROYABLE_BASE DestroyFn destroy

typedef struct Destroyable
{
    DESTROYABLE_BASE;
} Destroyable;

void destroy(Destroyable* what);
void destroyStatic(Destroyable* what);
void nullDestroy(Destroyable* what);

#endif // DESTROYABLE_H
