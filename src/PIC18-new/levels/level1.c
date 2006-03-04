#include "level1.h"
#include "boring.h"
#include "cloud.h"
#include "chase.h"
#include "enemy.h"
#include "chain.h"
#include "random-shoot.h"
#include "projectile.h"
#include "explosion.h"
#include "f16.anim.inl"
#include "plasma.anim.inl"

static void killEnemy(Enemy* who, World* world)
{
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_SMALL, 5);
    alignCenter(e, who);

    addUpdateable(world, e);
}

static void impactProjectile(Projectile* p, Entity* e, World* world)
{
    addUpdateable(world, createExplosion(p->position, EXPLOSION_TINY, 10));
}

static void spawnF(World* w)
{
    Behavior** bs;
    Enemy* e;

    Projectile* p = createProjectile(plasmaAnimation(), 0,
                                     createBoring(makeWhole(5)),
                                     1,
                                     makePoint(0, 0),
                                     impactProjectile);


    bs = newArray(Behavior*, 2);
    bs[0] = createBoring(makeWhole(2));
    bs[1] = createRandomShoot(500);

    e = createEnemy(f16Animation(), 0, 
                    createChainBehavior(bs, 2),
                    2, 
                    makePoint(80, -40),
                    p,
                    killEnemy);
    addEnemy(w, e);
}

#include "level1.lvl.inl"
