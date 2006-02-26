#ifndef BULLET_H
#define BULLET_H

#include "common.h"
#include "projectile.h"

typedef struct Bullet
{
    PROJECTILE_BASE;
} Bullet;

Bullet* createBullet(Point where, int8 speed);

#endif // BULLET_H
