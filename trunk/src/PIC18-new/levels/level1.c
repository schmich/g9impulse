#include "level1.h"
#include "boring.h"
#include "cloud.h"
#include "enemy.h"
#include "chain.h"
#include "chase.h"
#include "random-shoot.h"
#include "lock-on.h"
#include "projectile.h"
#include "explosion.h"
#include "f16.anim.inl"
#include "f18.anim.inl"
#include "plasma.anim.inl"
#include "fireball.anim.inl"

static void killEnemy(Enemy* who, World* world)
{
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_SMALL, 5);
    alignCenter(e, who);

    addUpdateable(world, e);
}

static void impactProjectile(Projectile* p, Sprite* s, World* world)
{
    addUpdateable(world, createExplosion(p->position, EXPLOSION_TINY, 10));
}

static void spawnPlasma(Projectile** p)
{
    *p = createProjectile(plasmaAnimation(), 0,
                          createBoring(makeWhole(5)),
                          1,
                          makePoint(0, 0),
                          impactProjectile);
}

static void spawnFireball(Projectile** p)
{
    *p = createProjectile(fireballAnimation(), 0,
                          createBoring(makeWhole(5)),
                          1,
                          makePoint(0, 0),
                          impactProjectile);
}

static void spawnF16(World* w, Point where)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 2);
    bs[0] = createBoring(makeWhole(2));
    bs[1] = createRandomShoot(500);

    e = createEnemy(f16Animation(), 0, 
                    createChainBehavior(bs, 2),
                    2, 
                    where,
                    spawnPlasma,
                    killEnemy);
    addEnemy(w, e);
}

static void spawnF18(World* w, Point where)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 2);
    bs[0] = createChase(2, 1);
    bs[1] = createLockOn(800);

    e = createEnemy(f18Animation(), 0, 
                    createChainBehavior(bs, 2),
                    1, 
                    where,
                    spawnFireball,
                    killEnemy);
    addEnemy(w, e);
}

static void spawnManyF18(World* w, int16 x, bool inc)
{
    int16 mult;
    if (inc)
        mult = 1;
    else
        mult = -1;

    spawnF18(w, makePoint(x + mult * 0,  -23));
    spawnF18(w, makePoint(x + mult * 15, -46));
    spawnF18(w, makePoint(x + mult * 30, -69));
}

static void spawnManyF16(World* w, int16 x, bool inc)
{
    int16 mult;
    if (inc)
        mult = 1;
    else
        mult = -1;

    spawnF16(w, makePoint(x + mult * 0,  -50));
    spawnF16(w, makePoint(x + mult * 20, -100));
    spawnF16(w, makePoint(x + mult * 40, -150));
}

static void spawnLineF16(World* w, int16 x, uint8 num)
{
    uint8 i;
    for (i = 0; i < num; ++i)
        spawnF16(w, makePoint(x + i*20, -50));
}

#include "level1.lvl.inl"
