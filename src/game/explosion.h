#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "common.h"
#include "updateable.h"

#define EXPLOSION_TINY   0
#define EXPLOSION_SMALL  1
#define EXPLOSION_MEDIUM 2
#define EXPLOSION_LARGE  3
#define EXPLOSION_HUGE   4

typedef struct Explosion
{
    UPDATEABLE_BASE;
} Explosion;

Explosion* createExplosion(Point where, uint8 size, uint8 frameDelay);

#endif // EXPLOSION_H
