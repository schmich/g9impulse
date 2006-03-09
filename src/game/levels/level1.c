#include "level1.h"
#include "cloud.h"
#include "boring.h"
#include "hover.h"
#include "direct.h"
#include "enemy.h"
#include "chain.h"
#include "chase.h"
#include "animator.h"
#include "roll.h"
#include "random-shoot.h"
#include "lock-on.h"
#include "projectile.h"
#include "explosion.h"
#include "powerup.h"
#include "f16.anim.inl"
#include "f18.anim.inl"
#include "mig.anim.inl"
#include "chopper.anim.inl"
#include "plasma.anim.inl"
#include "fireball.anim.inl"
#include "powerup.anim.inl"
#include "player.h"
#include "world.h"

static int16 abs(int16 x)
{
    if (x < 0)
        return -x;
    else
        return x;
}

static void spawnPowerup(World* world, Point where)
{
    Artifact* a;
    Behavior** bs;

    bs = newArray(Behavior*, 3);
    bs[0] = createBoring(makeWhole(1));
    bs[1] = createAnimator(20, FOREVER);

    a = createArtifact(powerupAnimation(), 0,
                       createChainBehavior(bs, 2),
                       where,
                       addHealth);

    addArtifact(world, a);
}

static void killEnemy(Enemy* who, World* world)
{
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_SMALL, 4);
    setSpriteCenter(e, spriteCenter(who));

    addUpdateable(world, e);
}

static void impactProjectile(Projectile* p, Sprite* s, World* world)
{
    addUpdateable(world, createExplosion(p->position, EXPLOSION_TINY, 10));
}

static void spawnPlasma(Entity* who, World* world, Projectile** p)
{
    Point tip = spriteCenterBottom(who);
    Point center = spriteCenter(world->player);

    if (abs(tip.y - center.y) > abs(tip.x - center.x))
    {
        *p = createProjectile(plasmaAnimation(), 0,
                              createDirect(tip, spriteCenter(world->player), 4),
                              1,
                              makePoint(0, 0),
                              impactProjectile);
    }
    else
    {
        *p = NULL;
    }
}

static void spawnFireball(Entity* who, World* world, Projectile** p)
{
    *p = createProjectile(fireballAnimation(), 0,
                          createBoring(makeWhole(5)),
                          1,
                          makePoint(0, 0),
                          impactProjectile);
}

static void spawnChopper(World* w, Point where)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 3);
    bs[0] = createHover(75, 250);
    bs[1] = createAnimator(1, FOREVER);
    bs[2] = createRandomShoot(300);

    e = createEnemy(chopperAnimation(), 0, 
                    createChainBehavior(bs, 3),
                    2,
                    where,
                    spawnPlasma,
                    killEnemy);
    addEnemy(w, e);
}

static void spawnF16(World* w, Point where)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 3);
    bs[0] = createBoring(makeWhole(2));
    bs[1] = createRandomShoot(300);
    bs[2] = createAnimator(3, FOREVER);

    e = createEnemy(f16Animation(), 0, 
                    createChainBehavior(bs, 3),
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

    bs = newArray(Behavior*, 3);
    bs[0] = createChase(2, 1);
    bs[1] = createRoll(where.x);
    bs[2] = createLockOn(800);

    e = createEnemy(f18Animation(), 0, 
                    createChainBehavior(bs, 3),
                    1, 
                    where,
                    spawnFireball,
                    killEnemy);
    addEnemy(w, e);
}

static void spawnMig(World* w, Point where)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 2);
    bs[0] = createBoring(makeWhole(3));
    bs[1] = createRandomShoot(300);

    e = createEnemy(migAnimation(), 0, 
                    createChainBehavior(bs, 2),
                    5, 
                    where,
                    spawnFireball,
                    killEnemy);
    addEnemy(w, e);
}

static void spawnF18Cut(World* w, Point where, bool right)
{
    Behavior** bs;
    Enemy* e;

    Point target = makePoint(right ? SCREEN_WIDTH : 0, 150);

    bs = newArray(Behavior*, 3);
    bs[0] = createDirect(where, target, 2);
    bs[1] = createRoll(where.x);
    bs[2] = createRandomShoot(500);

    e = createEnemy(f18Animation(), 0, 
                    createChainBehavior(bs, 3),
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
