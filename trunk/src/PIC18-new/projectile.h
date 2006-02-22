#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "common.h"
#include "updateable.h"

#define PROJECTILE_BASE UPDATEABLE_BASE; \
                        uint8 damage

typedef struct Projectile
{
    PROJECTILE_BASE;
} Projectile;

#endif // PROJECTILE_H
