#include "level1.h"
#include "cloud.h"
#include "boring.h"
#include "strafing.h"
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
#include "proximity-explosion.h"
#include "powerup.h"
#include "f16.anim.inl"
#include "f18.anim.inl"
#include "mig.anim.inl"
#include "chopper.anim.inl"
#include "tank.anim.inl"
#include "ship.anim.inl"
#include "plasma.anim.inl"
#include "fireball.anim.inl"
#include "powerup.anim.inl"
#include "missile.anim.inl"
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
    bs[0] = createBoring(2, 1);
    bs[1] = createAnimator(7, FOREVER);

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
    bs[1] = createAnimator(7, FOREVER);

    a = createArtifact(weaponPowerupAnimation(), 0,
                       createChainBehavior(bs, 2),
                       where,
                       powerupWeapon);

    addArtifact(world, a);
}

static void spawnNukePowerup(World* world, Point where)
{
    Artifact* a;
    Behavior** bs;

    bs = newArray(Behavior*, 3);
    bs[0] = createBoring(2, 1);
    bs[1] = createAnimator(7, FOREVER);

    a = createArtifact(nukePowerupAnimation(), 0,
                       createChainBehavior(bs, 2),
                       where,
                       powerupNuke);

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

static void killShip(Enemy* who, World* world)
{
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_LARGE, 4);
    setSpriteCenter(e, spriteCenter(who));

    addUpdateable(world, e);
}

static void killEnemy(Enemy* who, World* world)
{
    Player* p = world->player;
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_SMALL, 4);
    int16 randMax;
    uint8 upgrades;
    uint16 faced;

    setSpriteCenter(e, spriteCenter(who));

    addUpdateable(world, e);

    faced = p->stats->enemiesFaced;
    upgrades = weaponUpgrades(p);
    if (upgrades == 0 && faced <= 30)
    {
        randMax = 30 - faced;
        if (randMax < 0)
            randMax = 1;

        if (random(1, randMax) == 1)
        {
            spawnWeaponPowerup(world, spriteCenter(who));
        }
    }
    else if (upgrades < 1 && (faced > 30 && faced <= 60))
    {
        randMax = 60 - p->stats->enemiesFaced;
        if (randMax < 0)
            randMax = 1;

        if (random(1, randMax) == 1)
        {
            spawnWeaponPowerup(world, spriteCenter(who));
        }
    }
    else if (upgrades < 2 && (faced > 60 && faced <= 90))
    {
        randMax = 90 - p->stats->enemiesFaced;
        if (randMax < 0)
            randMax = 1;

        if (random(1, randMax) == 1)
        {
            spawnWeaponPowerup(world, spriteCenter(who));
        }
    }
    else if (random(0, 100) < 15)
    {
        if (random(0, 10) < 8)
            spawnHealthPowerup(world, spriteCenter(who));
        else
            spawnNukePowerup(world, spriteCenter(who));
    }
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

static void impactMissile(Projectile* p, Sprite* s, World* world)
{
    addUpdateable(world, createExplosion(p->position, EXPLOSION_SMALL, 5));
}

static void spawnPellet(Entity* who, World* world, Point where)
{
    Projectile* p;
    Point tip = spriteCenterBottom(who);
    Point center = spriteCenter(world->player);

    if (tip.y < center.y)
    {
        p = createProjectile(pelletAnimation(), 0,
                             createDirect(tip, center, 4),
                             1,
                             makePoint(0, 0),
                             impactProjectile,
                             false);

        setSpriteCenterBottom(p, where);
        addEnemyProjectile(world, p);
    }
}

static void spawnPlasma(Entity* who, World* world, Point where)
{
    Projectile* p;
    Behavior** bs;
    Point tip = spriteCenterBottom(who);
    Point center = spriteCenter(world->player);

    bs = newArray(Behavior*, 2);
    bs[0] = createDirect(tip, center, 4);
    bs[1] = createAnimator(4, FOREVER);

    if (abs(tip.y - center.y) > abs(tip.x - center.x))
    {
        p = createProjectile(plasmaAnimation(), 0,
                             createChainBehavior(bs, 2),
                             1,
                             makePoint(0, 0),
                             impactProjectile,
                             false);

        setSpriteCenterBottom(p, where);
        addEnemyProjectile(world, p);
    }
}

static void spawnFireball(Entity* who, World* world, Point where)
{
    Projectile* p = createProjectile(fireballAnimation(), 0,
                                     createBoring(5, 1),
                                     1,
                                     makePoint(0, 0),
                                     impactProjectile,
                                     false);

    setSpriteCenterBottom(p, where);
    addEnemyProjectile(world, p);
}

static void spawnProximityMissile(Entity* who, World* world, Point where)
{
    Behavior** bs;
    Projectile* p;

    bs = newArray(Behavior*, 2);
    bs[0] = createBoring(4, 1);
    bs[1] = createProximityExplosion(world->player);

    p = createProjectile(missileAnimation(), 0,
                         createChainBehavior(bs, 2),
                         0,
                         makePoint(0, 0),
                         nullImpact,
                         false);

    setSpriteCenterBottom(p, where);
    addEnemyProjectile(world, p);
}

static void spawnSeekMissile(Entity* who, World* world, Point where)
{
    Behavior** bs;
    Projectile* p;

    bs = newArray(Behavior*, 2);
    bs[0] = createChase(4, 1);
    bs[1] = createRoll(0);

    p = createProjectile(missileAnimation(), 0,
                         createChainBehavior(bs, 2),
                         3,
                         makePoint(0, 0),
                         impactMissile,
                         false);

    setSpriteCenterBottom(p, where);
    addEnemyProjectile(world, p);
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
                    25,
                    makePoint(x, 0),
                    spawnPlasma,
                    killEnemy);

    e->position.y = -(int16)spriteHeight(e) - dy;
    
    addEnemy(w, e);
}

static void spawnShip(World* w, int16 x, int16 dy)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 3);
    bs[0] = createBoring(1, 2);
    bs[1] = createStrafing(1, 2);
    bs[2] = createRandomShoot(400);

    e = createEnemy(shipAnimation(), 0,
                    createChainBehavior(bs, 3),
                    10,
                    makePoint(x, 0),
                    spawnPellet,
                    killShip);

    e->position.y = -(int16)spriteHeight(e) - dy;
    e->ground = true;

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
                    10,
                    makePoint(x, 0),
                    spawnPlasma,
                    killEnemy);

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
                    5, 
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
                    5,
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
    bs[0] = createBoring(2, 1);
    bs[1] = createRandomShoot(300);

    e = createEnemy(migAnimation(), 0, 
                    createChainBehavior(bs, 2),
                    30,
                    makePoint(x, 0),
                    spawnSeekMissile,
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
                    20,
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
                    5, 
                    makePoint(x, 0),
                    spawnFireball,
                    killEnemy);

    e->position.y = -(int16)spriteHeight(e) - dy;

    bs[0] = createDirect(makePoint(x, e->position.y), target, 2);

    addEnemy(w, e);
}

static void spawnF18Kamikaze(World* w, int16 x, int16 dy, uint8 speed)
{
    Behavior** bs;
    Enemy* e;

    bs = newArray(Behavior*, 2);
    bs[0] = createChase(speed, 1);
    bs[1] = createRoll(x);

    e = createEnemy(f18Animation(), 0,
                    createChainBehavior(bs, 2),
                    5, 
                    makePoint(x, 0),
                    nullProjectileSpawn,
                    killEnemy);

    e->fire = nullFire;
    e->position.y = -(int16)spriteHeight(e) - dy;

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
