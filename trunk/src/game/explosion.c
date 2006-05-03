#include "explosion.h"
#include "animator.h"
#include "explosion.anim.inl"

static void destroyExplosion(Explosion* e)
{
    destroy(e->behavior);
}

Explosion* createExplosion(Point where, uint8 size, uint8 frameDelay)
{
    Explosion* e = new(Explosion);
    e->destroy = destroyExplosion;
    e->behavior = createAnimator(frameDelay, 1);
    e->position = where;

    switch (size)
    {
        case EXPLOSION_TINY:
            e->animation = tinyExplosionAnimation();
            break;

        case EXPLOSION_SMALL:
            e->animation = smallExplosionAnimation();
            break;

        case EXPLOSION_MEDIUM:
            e->animation = mediumExplosionAnimation();
            break;

        case EXPLOSION_LARGE:
            e->animation = largeExplosionAnimation();
            break;

        case EXPLOSION_HUGE:
            e->animation = hugeExplosionAnimation();
            break;
    }

    animationBeginning(e);

    return e;
}
