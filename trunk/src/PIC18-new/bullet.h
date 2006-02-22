#ifndef BULLET_H
#define BULLET_H

#include "common.h"
#include "projectile.h"

typedef struct Bullet
{
    PROJECTILE_BASE;
} Bullet;

Bullet* createBullet(Point where);

#endif // BULLET_H
