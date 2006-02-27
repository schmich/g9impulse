#ifndef FIREBALL_H
#define FIREBALL_H

#include "common.h"
#include "projectile.h"

typedef struct Fireball
{
    PROJECTILE_BASE;
} Fireball;

Fireball* createFireball(Point where, int8 speed);

#endif // FIREBALL_H
