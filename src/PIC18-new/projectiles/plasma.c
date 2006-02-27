#include "plasma.h"
#include "boring.h"
#include "chain.h"
#include "seek.h"
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
    Behavior** chain;
    Animation* anim;

    Plasma* p = new(Plasma);
    p->destroy = destroyPlasma;

    chain = newArray(Behavior*, 2);
    chain[0] = createBoring(speed);
    chain[1] = createSeek();
    p->behavior = createChainBehavior(chain, 2);
 
    p->impact = impactPlasma;
    p->position = where;
    p->damage = 1;

    p->animation = plasmaAnimation();

    animationBeginning(p);

    return p;
}
