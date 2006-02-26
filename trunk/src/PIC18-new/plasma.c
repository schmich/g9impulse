#include "plasma.h"
#include "behavior.h"
#include "explosion.h"
#include "plasma.anim.inl"

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
