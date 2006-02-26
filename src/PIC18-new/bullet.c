#include "bullet.h"
#include "behavior.h"
#include "explosion.h"

static Animation theAnimation;
static bool theInitAnimation = false;

static Animation* bulletAnimation(void)
{
    if (!theInitAnimation)
    {
        theAnimation.numImages = 1;
        theAnimation.images = newArray(Image, theAnimation.numImages);

        theAnimation.images[0] = makeImage(0x000D97B2, 2, 6);

        theInitAnimation = true;
    }

    return &theAnimation;
}

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
