#include "player.h"
#include "input.h"
#include "explosion.h"
#include "world.h"
#include "projectile.h"
#include "animator.h"
#include "boring.h"
#include "chain.h"
#include "nuke.h"
#include "direct.h"
#include "beam.h"
#include "player.anim.inl"
#include "nuke.anim.inl"
#include "fireball.anim.inl"

#define MAX_MOMENTUM_Y 13
#define MAX_MOMENTUM_X 10
#define MAX_NUKE_COOLDOWN 50

static void destroyPlayer(Player* p)
{
    destroy(p->behavior);
    destroy(p->engine);
    destroy(p->stats);
}

static void killPlayer(Player* who, World* world)
{
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_HUGE, 7);
    setSpriteCenter(e, spriteCenter(who));

    addUpdateable(world, e);

    if (who->lives > 0)
        --who->lives;

    ++who->stats->livesLost;

    //
    // uncomment to decrement weapons
    // if the player dies
    //
    /*
    uint8 i;
    for (i = 0; i < 2; ++i)
        if (who->weaponLevel[i] > 0)
            --who->weaponLevel[i];

    switchWeapon(who);
    */
}

static void firePlayer(Entity* player, World* world)
{
    player->spawnProjectile(player,
                            world,
                            spriteCenterTop(player));
}

static void impactProjectile(Projectile* p, Sprite* s, World* world)
{
    addUpdateable(world, createExplosion(p->position, EXPLOSION_TINY, 10));
}

static void spawnBullet(Entity* player, World* world, Point where)
{
    Projectile* p = createProjectile(bulletAnimation(), 0,
                                     createBoring(-9, 1),
                                     5,
                                     makePoint(0, 0),
                                     impactProjectile,
                                     false);
    setSpriteCenterTop(p, where);
    addPlayerProjectile(world, p);
}

static void spawnDoubleBullet(Entity* player, World* world, Point where)
{
    Projectile* p = createProjectile(doubleBulletAnimation(), 0,
                                     createBoring(-9, 1),
                                     8,
                                     makePoint(0, 0),
                                     impactProjectile,
                                     false);
    setSpriteCenterTop(p, where);
    addPlayerProjectile(world, p);
}

static void spawnLaser(Entity* player, World* world, Point where)
{
    Projectile* p = createProjectile(laserAnimation(), 0,
                                     createBoring(-9, 1),
                                     12,
                                     makePoint(0, 0),
                                     impactProjectile,
                                     false);

    setSpriteCenterTop(p, where);
    addPlayerProjectile(world, p);
}

static void spawnWave(Entity* player, World* world, Point where)
{
    Projectile* p = createProjectile(waveAnimation(), 0,
                                     createBoring(-9, 1),
                                     20,
                                     makePoint(0, 0),
                                     impactProjectile,
                                     false);

    setSpriteCenterTop(p, where);
    addPlayerProjectile(world, p);
}

static void spawnRedBeam(Entity* who, World* world, Point where)
{
    Behavior** bs;
    Player* player = who;
    Projectile* p;

    bs = newArray(Behavior*, 2);
    bs[0] = createAnimator(6, 1);
    bs[1] = createBeam(player);

    p = createProjectile(redBeamAnimation(), 0,
                         createChainBehavior(bs, 2),
                         1,
                         makePoint(0, 0),
                         nullImpact,
                         true);

    setSpriteCenterTop(p, where);
    addPlayerProjectile(world, p);
}

static void spawnGreenBeam(Entity* who, World* world, Point where)
{
    Behavior** bs;
    Player* player = who;
    Projectile* p;

    bs = newArray(Behavior*, 2);
    bs[0] = createAnimator(6, 1);
    bs[1] = createBeam(player);

    p = createProjectile(greenBeamAnimation(), 0,
                         createChainBehavior(bs, 2),
                         2,
                         makePoint(0, 0),
                         nullImpact,
                         true);

    setSpriteCenterTop(p, where);
    addPlayerProjectile(world, p);
}

static void spawnSpread2(Entity* player, World* world, Point where)
{
    Projectile* p;
    Point target = where;

    target.y -= 10;
    target.x -= 1;
    p = createProjectile(fireballAnimation(), 7,
                         createDirect(where, target, 7),
                         4,
                         makePoint(0, 0),
                         impactProjectile,
                         false);

    setSpriteCenterTop(p, where);
    addPlayerProjectile(world, p);

    target.x += 2;
    p = createProjectile(fireballAnimation(), 9,
                         createDirect(where, target, 7),
                         4,
                         makePoint(0, 0),
                         impactProjectile,
                         false);

    setSpriteCenterTop(p, where);
    addPlayerProjectile(world, p);
}

static void spawnSpread3(Entity* player, World* world, Point where)
{
    Projectile* p = createProjectile(fireballAnimation(), 8,
                                     createBoring(-7, 1),
                                     4,
                                     makePoint(0, 0),
                                     impactProjectile,
                                     false);

    setSpriteCenterTop(p, where);
    addPlayerProjectile(world, p);

    spawnSpread2(player, world, where);
}

static void spawnSpread5(Entity* player, World* world, Point where)
{
    Projectile* p;
    Point target = where;

    target.x -= 1;
    target.y -= 5;

    p = createProjectile(fireballAnimation(), 6,
                         createDirect(where, target, 7),
                         3,
                         makePoint(0, 0),
                         impactProjectile,
                         false);

    setSpriteCenterTop(p, where);
    addPlayerProjectile(world, p);

    target.x += 2;
    p = createProjectile(fireballAnimation(), 10,
                         createDirect(where, target, 7),
                         3,
                         makePoint(0, 0),
                         impactProjectile,
                         false);

    setSpriteCenterTop(p, where);
    addPlayerProjectile(world, p);

    spawnSpread3(player, world, where);
}

static void spawnNuke(Entity* player, World* world, Point where)
{
    Behavior** bs;
    Projectile* p;

    bs = newArray(Behavior*, 2);
    bs[0] = createBoring(-6, 1);
    bs[1] = createNuke(player->position.y / 3);

    p = createProjectile(nukeAnimation(), 0,
                         createChainBehavior(bs, 2),
                         0,
                         makePoint(0, 0),
                         nullImpact,
                         true);

    setSpriteCenterTop(p, where);
    addPlayerProjectile(world, p);

    ++((Player*)player)->stats->nukesFired;
}

static void switchWeapon(Player* who)
{
    switch (who->weaponClass)
    {
        //
        // single shot
        //
        case 0:
            switch (who->weaponLevel[0])
            {
                case 0:
                    who->spawnProjectile = spawnBullet;
                    who->maxCooldown = 9;
                    who->heatup = 1;
                    break;

                case 1:
                    who->spawnProjectile = spawnDoubleBullet;
                    who->maxCooldown = 7;
                    who->heatup = 2;
                    break;

                case 2:
                    who->spawnProjectile = spawnLaser;
                    who->maxCooldown = 6;
                    who->heatup = 1;
                    break;

                default:
                case 3:
                    who->spawnProjectile = spawnWave;
                    who->maxCooldown = 4;
                    who->heatup = 1;
                    break;
            }
            break;

        //
        // spread shot
        //
        case 1:
            switch (who->weaponLevel[1])
            {
                case 0:
                    who->spawnProjectile = spawnSpread2;
                    who->maxCooldown = 10;
                    who->heatup = 2;
                    break;

                case 1:
                    who->spawnProjectile = spawnSpread3;
                    who->maxCooldown = 5;
                    who->heatup = 3;
                    break;

                default:
                case 2:
                    who->spawnProjectile = spawnSpread5;
                    who->maxCooldown = 2;
                    who->heatup = 4;
                    break;
            }
            break;

        //
        // beam
        //
        case 2:
            switch (who->weaponLevel[2])
            {
                case 0:
                    who->spawnProjectile = spawnRedBeam;
                    who->maxCooldown = 5;
                    who->heatup = 28;
                    break;

                default:
                case 1:
                    who->spawnProjectile = spawnGreenBeam;
                    who->maxCooldown = 5;
                    who->heatup = 28;
                    break;
            }
            break;
    }
}

static uint8 updatePlayer(Player* who, World* world)
{
    Projectile* bullet;
    Input* input = getInputStatus();
    Input* event = getInputEvent();
    uint8 width;
    uint8 height;
    SpawnFn currSpawn;

    if (event->selectPressed)
    {
        if (++who->weaponClass == 3)
            who->weaponClass = 0;

        switchWeapon(who);
    }

    if (event->buttonBPressed)
    {
        if (who->heat < 6)
        {
            fire(who, world);

            who->heat += who->heatup;
            who->cooldown = who->maxCooldown;
        }
    }
    else
    {
        if (who->heat > 0)
        {
            if (--who->cooldown == 0)
            {
                --who->heat;
                who->cooldown = who->maxCooldown;
            }
        }
    }

    if (event->buttonAPressed)
    {
        if (who->nukes > 0 && who->nukeCooldown == 0)
        {
            currSpawn = who->spawnProjectile;
            who->spawnProjectile = spawnNuke;

            fire(who, world);

            who->spawnProjectile = currSpawn;
            --who->nukes;

            who->nukeCooldown = MAX_NUKE_COOLDOWN;
        }
    }
    else
    {
        if (who->nukeCooldown > 0)
            --who->nukeCooldown;
    }

    if (input->leftPressed)
    {
        who->position.x -= 1;

        if (--who->momentum.x < -(MAX_MOMENTUM_X / 2))
        {
            if (who->momentum.x < -MAX_MOMENTUM_X)
                who->momentum.x = -MAX_MOMENTUM_X;

            rollLeft(who);
        }
    }
    else if (input->rightPressed)
    {
        who->position.x += 1;

        if (++who->momentum.x > (MAX_MOMENTUM_X / 2))
        {
            if (who->momentum.x > MAX_MOMENTUM_X)
                who->momentum.x = MAX_MOMENTUM_X;

            rollRight(who);
        }
    }
    else
    {
        if (who->momentum.x < 0)
            ++who->momentum.x;
        else if (who->momentum.x > 0)
            --who->momentum.x;

        if ((who->momentum.x > -(MAX_MOMENTUM_X / 2)) &&
            (who->momentum.x < (MAX_MOMENTUM_X / 2)))
            noRoll(who);
    }

    if (input->upPressed)
    {
        if (--who->momentum.y < -MAX_MOMENTUM_Y)
            who->momentum.y = -MAX_MOMENTUM_Y;
    }
    else if (input->downPressed)
    {
        if (++who->momentum.y > MAX_MOMENTUM_Y)
            who->momentum.y = MAX_MOMENTUM_Y;
    }
    else
    {
        if (who->momentum.y < 0)
            ++who->momentum.y;
        else if (who->momentum.y > 0)
            --who->momentum.y;
    }

    who->position.x += who->momentum.x / 4;
    who->position.y += who->momentum.y / 3;

    if (who->position.x < 0)
    {
        who->position.x = 0;
    }
    else
    {
        width = spriteWidth(who);

        if ((who->position.x + width) > SCREEN_WIDTH)
            who->position.x = SCREEN_WIDTH - width;
    }

    if (who->position.y < 0)
    {
        who->position.y = 0;
    }
    else
    {
        height = spriteHeight(who);

        if ((who->position.y + height) > (SCREEN_HEIGHT + 5))
            who->position.y = (SCREEN_HEIGHT + 5) - height;
    }

    update(who->engine, world);

    return UPDATE_KEEP;
}

void respawnPlayer(Player* who)
{
    who->health = 6;
    who->momentum = makePoint(0, 0);
    who->heat = 0;
    who->cooldown = who->maxCooldown;
    animationBeginning(who);
    setSpriteCenterTop(who, screenCenterBottom());
}

Player* createPlayer(Point where)
{
    Behavior** bs;

    Player* player = new(Player);
    player->destroy = destroyPlayer;
    player->position = where;
    player->kill = killPlayer;
    player->fire = firePlayer;

    player->behavior = createBehavior(updatePlayer);

    player->animation = playerAnimation();
    animationBeginning(player);

    player->engine = createEngine(player);

    player->momentum = makePoint(0, 0);
    player->heat = 0;

    player->lives = 3;

    player->nukeCooldown = MAX_NUKE_COOLDOWN;
    player->nukes = 3;

    player->stats = createStats();

    player->weaponLevel[0] =
    player->weaponLevel[1] =
    player->weaponLevel[2] = 0;
    player->weaponClass = 0;

    switchWeapon(player);

    respawnPlayer(player);

    return player;
}

void rollLeft(Player* who)
{
    who->currentFrame = 2;
}

void rollRight(Player* who)
{
    who->currentFrame = 1;
}

void noRoll(Player* who)
{
    who->currentFrame = 0;
}

void upgradeWeapon(Player* who)
{
    ++who->weaponLevel[who->weaponClass];
    switchWeapon(who);
}

void enemyKilled(Player* who, Enemy* enemy, bool collided)
{
    uint16 score = enemy->maxHealth * 7;
    if (collided)
        score >>= 1;

    ++who->stats->kills;
    updateScore(who, score);
}

bool damagePlayer(Player* who, uint8 amount)
{
    who->stats->healthLost += amount;
    return damage(who, amount);
}

void updateScore(Player* who, int16 change)
{
    if (change > 0)
        who->stats->score += change;
    else
    {
        if (who->stats->score < -change)
            who->stats->score = 0;
        else
            who->stats->score += change;
    }
}

void drawPlayer(Player* who)
{
    drawSprite(who);
    drawSprite(who->engine);
}

uint8 weaponUpgrades(Player* who)
{
    return who->weaponLevel[0]
         + who->weaponLevel[1]
         + who->weaponLevel[2];
}
