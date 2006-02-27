#include "bullet.h"
#include "boring.h"
#include "explosion.h"
#include "bullet.anim.inl"

static void destroyBullet(Bullet* b)
{
    destroy(b->behavior);
}

static void impactBullet(Bullet* b, Entity* who, World* world)
{
    addUpdateable(world, createExplosion(b->position, EXPLOSION_TINY, 10));
}

Bullet* createBullet(Point where, int8 speed)
{
    Animation* anim;

    Bullet* b = new(Bullet);
    b->destroy = destroyBullet;
    b->behavior = createBoring(speed);
    b->impact = impactBullet;
    b->position = where;
    b->damage = 1;

    b->animation = bulletAnimation();

    animationBeginning(b);

    return b;
}
