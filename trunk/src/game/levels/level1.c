#include "level1.h"
#include "cloud.h"
#include "boring.h"
#include "hover.h"
#include "direct.h"
#include "enemy.h"
#include "chain.h"
#include "chase.h"
#include "facing.h"
#include "animator.h"
#include "spawner.h"
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
#include "tank.anim.inl"
#include "plasma.anim.inl"
#include "fireball.anim.inl"
#include "powerup.anim.inl"
#include "missle.anim.inl"
#include "player.h"
#include "world.h"

static void spawnRandomExplosions(World* world,
                                  Point where,
                                  int16 spaceRange,
                                  int16 timeRange,
                                  uint8 count)
{
    uint8 i;
    Explosion* e;

    spaceRange = abs(spaceRange);
    timeRange = abs(timeRange);

    for (i = 0; i < count; ++i)
    {
        e = createExplosion(makePoint(0, 0), EXPLOSION_SMALL, 4);
        setSpriteCenter(e, where);

        e->position.x += random(-spaceRange, spaceRange);
        e->position.y += random(-spaceRange, spaceRange);
        
        addUpdateable(world,
                      createSpawner(e,
                                    random(0, timeRange),
                                    SPAWNER_UPDATEABLE));
    }
}

static void spawnHealthPowerup(World* world, Point where)
{
    Artifact* a;
    Behavior** bs;

    bs = newArray(Behavior*, 2);
    bs[0] = createBoring(1, 1);
    bs[1] = createAnimator(3, FOREVER);

    a = createArtifact(healthPowerupAnimation(), 0,
                       createChainBehavior(bs, 2),
                       where,
                       powerupHealth);

    addArtifact(world, a);
}

static void spawnWeaponPowerup(World* world, Point where)
{
    Artifact* a;
    Behavior** bs;

    bs = newArray(Behavior*, 3);
    bs[0] = createBoring(2, 1);
    bs[1] = createAnimator(10, FOREVER);

    a = createArtifact(weaponPowerupAnimation(), 0,
                       createChainBehavior(bs, 2),
                       where,
                       powerupWeapon);

    addArtifact(world, a);
}

static void killTank(Enemy* who, World* world)
{
    Behavior** bs;
    Updateable* crater;

    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_SMALL, 5);
    setSpriteCenter(e, spriteCenter(who));

    bs = newArray(Behavior*, 2);
    bs[0] = createBoring(1, 2);
    bs[1] = createAnimator(30, FOREVER);

    crater = createUpdateable(craterAnimation(), 0,
                              createChainBehavior(bs, 2),
                              makePoint(0, 0));

    setSpriteCenter(crater, spriteCenter(who));
 
    addUpdateable(world, e);
    addUnderlay(world, crater);
}

static void killEnemy(Enemy* who, World* world)
{
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_SMALL, 4);
    setSpriteCenter(e, spriteCenter(who));

    addUpdateable(world, e);

    if (random(0, 100) < 10)
        spawnHealthPowerup(world, spriteCenter(who));
}

static void killSpecialEnemy(Enemy* who, World* world)
{
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_SMALL, 4);
    setSpriteCenter(e, spriteCenter(who));

    addUpdateable(world, e);

    spawnWeaponPowerup(world, spriteCenter(who));
}

static void killLargeEnemy(Enemy* who, World* world)
{
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_MEDIUM, 6);
    setSpriteCenter(e, spriteCenter(who));

    spawnRandomExplosions(world, spriteCenter(who), 15, 50, 3);

    addUpdateable(world, e);
}

static void impactProjectile(Projectile* p, Sprite* s, World* world)
{
    addUpdateable(world, createExplosion(p->position, EXPLOSION_TINY, 10));
}

static void impactMissle(Projectile* p, Sprite* s, World* world)
{
    addUpdateable(world, createExplosion(p->position, EXPLOSION_SMALL, 5));
}

static void spawnPellet(Entity* who, World* world, Projectile** p)
{
    Point tip = spriteCenterBottom(who);
    Point center = spriteCenter(world->player);

    if (tip.y < center.y)
    {
        *p = createProjectile(pelletAnimation(), 0,
                              createDirect(tip, center, 4),
                              1,
                              makePoint(0, 0),
                              impactProjectile,
                              false);
    }
    else
    {
        *p = NULL;
    }
}

static void spawnPlasma(Entity* who, World* world, Projectile** p)
{
    Behavior** bs;
    Point tip = spriteCenterBottom(who);
    Point center = spriteCenter(world->player);

    bs = newArray(Behavior*, 2);
    bs[0] = createDirect(tip, center, 4);
    bs[1] = createAnimator(4, FOREVER);

    if (abs(tip.y - center.y) > abs(tip.x - center.x))
    {
        *p = createProjectile(plasmaAnimation(), 0,
                              createChainBehavior(bs, 2),
                              1,
                              makePoint(0, 0),
                              impactProjectile,
                              false);
    }
    else
    {
        *p = NULL;
    }
}

static void spawnFireball(Entity* who, World* world, Projectile** p)
{
    *p = createProjectile(fireballAnimation(), 0,
                          createBoring(5, 1),
                          1,
                          makePoint(0, 0),
                          impactProjectile,
                          false);
}

static void spawnMissle(Entity* who, World* world, Projectile** p)
{
    Behavior** bs;

    bs = newArray(Behavior*, 2);
    bs[0] = createChase(4, 1);
    bs[1] = createRoll(0);

    *p = createProjectile(missleAnimation(), 0,
                          createChainBehavior(bs, 2),
                          3,
                          makePoint(0, 0),
                          impactMissle,
                          false);
}

static void spawnChopper(World* w, int16 x, int16 dy, int16 yDest)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 3);
    bs[0] = createHover(yDest, 250);
    bs[1] = createAnimator(1, FOREVER);
    bs[2] = createRandomShoot(300);

    e = createEnemy(chopperAnimation(), 0, 
                    createChainBehavior(bs, 3),
                    4,
                    makePoint(x, 0),
                    spawnPlasma,
                    killEnemy);

    e->position.y = -(int16)spriteHeight(e) - dy;
    
    addEnemy(w, e);
}

static void spawnF16(World* w, int16 x, int16 dy, uint8 speed)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 3);
    bs[0] = createBoring(speed, 1);
    bs[1] = createRandomShoot(300);
    bs[2] = createAnimator(3, FOREVER);

    e = createEnemy(f16Animation(), 0, 
                    createChainBehavior(bs, 3),
                    2,
                    makePoint(x, 0),
                    spawnPlasma,
                    killEnemy);

    e->position.y = -(int16)spriteHeight(e) - dy;

    addEnemy(w, e);
}

static void spawnF16Special(World* w, int16 x, int16 dy, uint8 speed)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 3);
    bs[0] = createBoring(speed, 1);
    bs[1] = createRandomShoot(300);
    bs[2] = createAnimator(3, FOREVER);

    e = createEnemy(f16Animation(), 0, 
                    createChainBehavior(bs, 3),
                    2,
                    makePoint(x, 0),
                    spawnPlasma,
                    killSpecialEnemy);

    e->position.y = -(int16)spriteHeight(e) - dy;

    addEnemy(w, e);
}

static void spawnF18Chase(World* w, int16 x, int16 dy, uint8 speed)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 3);
    bs[0] = createChase(speed, 1);
    bs[1] = createRoll(x);
    bs[2] = createLockOn(800);

    e = createEnemy(f18Animation(), 0, 
                    createChainBehavior(bs, 3),
                    1, 
                    makePoint(x, 0),
                    spawnFireball,
                    killEnemy);

    e->position.y = -(int16)spriteHeight(e) - dy;

    addEnemy(w, e);
}

static void spawnF18Boring(World* w, int16 x, int16 dy, uint8 speed)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 3);
    bs[0] = createBoring(speed, 1);
    bs[1] = createRoll(x);
    bs[2] = createLockOn(800);

    e = createEnemy(f18Animation(), 0,
                    createChainBehavior(bs, 3),
                    1,
                    makePoint(x, 0),
                    spawnFireball,
                    killEnemy);

    e->position.y = -(int16)spriteHeight(e) - dy;

    addEnemy(w, e);
}

static void spawnMig(World* w, int16 x, int16 dy)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 2);
    bs[0] = createBoring(3, 1);
    bs[1] = createRandomShoot(300);

    e = createEnemy(migAnimation(), 0, 
                    createChainBehavior(bs, 2),
                    5,
                    makePoint(x, 0),
                    spawnMissle,
                    killLargeEnemy);

    e->position.y = -(int16)spriteHeight(e) - dy;

    addEnemy(w, e);
}

static void spawnTank(World* w, int16 x, int16 dy)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 3);
    bs[0] = createBoring(1, 2);
    bs[1] = createRandomShoot(400);
    bs[2] = createFacing();

    e = createEnemy(tankAnimation(), 0,
                    createChainBehavior(bs, 3),
                    4,
                    makePoint(x, 0),
                    spawnPellet,
                    killTank);

    e->position.y = -(int16)spriteHeight(e) - dy;
    e->ground = true;

    addEnemy(w, e);
}

static void spawnF18Cut(World* w, int16 x, int16 dy, int16 yTarget, bool right)
{
    Behavior** bs;
    Enemy* e;

    Point target = makePoint(right ? SCREEN_WIDTH : 0, yTarget);

    bs = newArray(Behavior*, 3);
    bs[1] = createRoll(x);
    bs[2] = createRandomShoot(500);

    e = createEnemy(f18Animation(), 0, 
                    createChainBehavior(bs, 3),
                    1, 
                    makePoint(x, 0),
                    spawnFireball,
                    killEnemy);

    e->position.y = -(int16)spriteHeight(e) - dy;

    bs[0] = createDirect(makePoint(x, e->position.y), target, 2);

    addEnemy(w, e);
}

static void spawnManyF18Cut(World* w, int16 x, int16 yTarget, bool right)
{
    int16 mult;
    if (right)
        mult = -1;
    else
        mult = 1;

    spawnF18Cut(w, x, 10, yTarget, right); 
    spawnF18Cut(w, x + mult * 25, 20, yTarget, right); 
    spawnF18Cut(w, x + mult * 50, 30, yTarget, right); 
}

static void spawnManyF18(World* w, int16 x, bool inc)
{
    int16 mult;
    if (inc)
        mult = 1;
    else
        mult = -1;

    /*spawnF18(w, makePoint(x + mult * 0,  -23));
    spawnF18(w, makePoint(x + mult * 15, -46));
    spawnF18(w, makePoint(x + mult * 30, -69));*/
}

static void spawnManyF16(World* w, int16 x, bool inc)
{
    int16 mult;
    if (inc)
        mult = 1;
    else
        mult = -1;

    /*spawnF16(w, makePoint(x + mult * 0,  -50));
    spawnF16(w, makePoint(x + mult * 20, -100));
    spawnF16(w, makePoint(x + mult * 40, -150));*/
}

static void spawnCloudUnder(World* w, int16 x, int16 dy, int8 speed, uint8 size)
{
    addUnderlay(w, createCloud(x, dy, speed, size));
}

static void spawnCloudOver(World* w, int16 x, int16 dy, int8 speed, uint8 size)
{
    addOverlay(w, createCloud(x, dy, speed, size));
}

#include "level1.lvl.inl"
