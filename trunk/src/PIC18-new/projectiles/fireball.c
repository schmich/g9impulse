#include "fireball.h"
#include "boring.h"
#include "explosion.h"
#include "fireball.anim.inl"

static void destroyFireball(Fireball* f)
{
    destroy(f->behavior);
}

static void impactFireball(Fireball* f, Entity* who, World* world)
{
    addUpdateable(world, createExplosion(f->position, EXPLOSION_TINY, 10));
}

Fireball* createFireball(Point where, int8 speed)
{
    Fireball* f = new(Fireball);
    f->destroy = destroyFireball;
    f->behavior = createBoring(speed);
 
    f->impact = impactFireball;
    f->position = where;
    f->damage = 1;

    f->animation = fireballAnimation();
    animationBeginning(f);

    return f;
}
