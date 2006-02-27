#ifndef PLASMA_H
#define PLASMA_H

#include "common.h"
#include "projectile.h"

typedef struct Plasma
{
    PROJECTILE_BASE;
} Plasma;

Plasma* createPlasma(Point where, int8 speed);

#endif // PLASMA_H
