#include "projectile.h"

static void destroyProjectile(Projectile* p)
{
    destroy(p->behavior);
}

Projectile* createProjectile(Animation* anim,
                             uint8 initFrame,
                             Behavior* behavior,
                             uint8 damage,
                             Point where,
                             ImpactFn onImpact)
{
    Projectile* p = new(Projectile);
    p->destroy = destroyProjectile;
    p->behavior = behavior;
    p->position = where;
    p->damage = damage;
    p->impact = onImpact;
    p->animation = anim;
    p->currentFrame = initFrame;

    return p;
}

void impact(Projectile* proj, Sprite* who, World* world)
{
    proj->impact(proj, who, world); 
}
