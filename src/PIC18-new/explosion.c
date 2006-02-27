#include "explosion.h"
#include "behavior.h"
#include "explosion.anim.inl"

static void destroyExplosion(Explosion* e)
{
    destroy(e->behavior);
}

Explosion* createExplosion(Point where, uint8 size, uint8 frames)
{
    Explosion* e = new(Explosion);
    e->destroy = destroyExplosion;
    e->behavior = createAnimator(frames);
    e->position = where;

    switch (size)
    {
        case EXPLOSION_TINY:
            e->animation = tinyAnimation();
            break;

        case EXPLOSION_SMALL:
            e->animation = smallAnimation();
            break;

        case EXPLOSION_MEDIUM:
            e->animation = mediumAnimation();
            break;

        case EXPLOSION_LARGE:
            e->animation = largeAnimation();
            break;
    }

    animationBeginning(e);

    return e;
}
