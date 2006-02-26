#include "plasma.h"
#include "behavior.h"
#include "explosion.h"

static Animation theAnimation;
static bool theInitAnimation = false;

static Animation* plasmaAnimation(void)
{
    if (!theInitAnimation)
    {
        theAnimation.numImages = 1;
        theAnimation.images = newArray(Image, theAnimation.numImages);

        theAnimation.images[0] = makeImage(0x000D97BB, 4, 7);

        theInitAnimation = true;
    }

    return &theAnimation;
}

static void destroyPlasma(Plasma* p)
{
    destroy(p->behavior);
}

static void impactPlasma(Plasma* p, Entity* who, World* world)
{
    addUpdateable(world, createExplosion(p->position, EXPLOSION_TINY, 10));
}

Plasma* createPlasma(Point where, int8 speed)
{
    Animation* anim;

    Plasma* p = new(Plasma);
    p->destroy = destroyPlasma;
    p->behavior = createBoring(speed);
    p->impact = impactPlasma;
    p->position = where;
    p->damage = 1;

    p->animation = plasmaAnimation();

    animationBeginning(p);

    return p;
}
