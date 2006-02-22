#ifndef DESTROYABLE_H
#define DESTROYABLE_H

#include "common.h"

struct Destroyable;
typedef void (*DestroyFn)(Destroyable* what);

#define DESTROYABLE_BASE \
    DestroyFn destroy;

typedef struct Destroyable
{
    DESTROYABLE_BASE;
} Destroyable;

void destroy(Destroyable* what);
void destroyStatic(Destroyable* what);
Destroyable* nullDestroy();

#endif // DESTROYABLE_H
