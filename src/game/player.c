#include "player.h"
#include "input.h"
#include "explosion.h"
#include "world.h"
#include "projectile.h"
#include "animator.h"
#include "boring.h"
#include "chain.h"
#include "nuke.h"
#include "beam.h"
#include "player.anim.inl"
#include "nuke.anim.inl"

#define MAX_MOMENTUM_Y 13
#define MAX_MOMENTUM_X 10
#define MAX_WEAPONS 5
#define INITIAL_MAX_COOLDOWN 9
#define INITIAL_HEATUP 1
#define MAX_NUKE_COOLDOWN 50

static void destroyPlayer(Player* p)
{
    destroy(p->behavior);
    destroy(p->engine);
}

static void killPlayer(Player* who, World* world)
{
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_HUGE, 7);
    setSpriteCenter(e, spriteCenter(who));

    addUpdateable(world, e);
}

static void firePlayer(Entity* player, World* world)
{
    Projectile* p;
    player->spawnProjectile(player, world, &p);

    if (p)
    {
        setSpriteCenterTop(p, spriteCenterTop(player));
        addPlayerProjectile(world, p);
    }
}

static void impactProjectile(Projectile* p, Sprite* s, World* world)
{
    addUpdateable(world, createExplosion(p->position, EXPLOSION_TINY, 10));
}

static void spawnBullet(Entity* player, World* world, Projectile** p)
{
    *p = createProjectile(bulletAnimation(), 0,
                          createBoring(-7, 1),
                          1,
                          makePoint(0, 0),
                          impactProjectile,
                          false);
}

static void spawnDoubleBullet(Entity* player, World* world, Projectile** p)
{
    *p = createProjectile(doubleBulletAnimation(), 0,
                          createBoring(-7, 1),
                          2,
                          makePoint(0, 0),
                          impactProjectile,
                          false);
}

static void spawnLaser(Entity* player, World* world, Projectile** p)
{
    *p = createProjectile(laserAnimation(), 0,
                          createBoring(-7, 1),
                          3,
                          makePoint(0, 0),
                          impactProjectile,
                          false);
}

static void spawnWave(Entity* player, World* world, Projectile** p)
{
    *p = createProjectile(waveAnimation(), 0,
                          createBoring(-7, 1),
                          4,
                          makePoint(0, 0),
                          impactProjectile,
                          false);
}

static void spawnBeam(Entity* who, World* world, Projectile** p)
{
    Behavior** bs;
    Player* player = who;

    bs = newArray(Behavior*, 2);
    bs[0] = createAnimator(6, 1);
    bs[1] = createBeam(player);

    *p = createProjectile(beamAnimation(), 0,
                          createChainBehavior(bs, 2),
                          1,
                          makePoint(0, 0),
                          nullImpact,
                          true);
}

static void spawnNuke(Entity* player, World* world, Projectile** p)
{
    Behavior** bs;

    bs = newArray(Behavior*, 2);
    bs[0] = createBoring(-6, 1);
    bs[1] = createNuke(player->position.y / 3);

    *p = createProjectile(nukeAnimation(), 0,
                          createChainBehavior(bs, 2),
                          0,
                          makePoint(0, 0),
                          nullImpact,
                          true);
}

static uint8 updatePlayer(Player* who, World* world)
{
    Projectile* bullet;
    Input* input = getInputStatus();
    Input* event = getInputEvent();
    uint8 width;
    uint8 height;
    SpawnFn currSpawn;

    if (event->buttonBPressed)
    {
        if (who->heat < 6)
        {
            fire(who, world);

            who->heat += who->heatup;
            who->cooldown = who->maxCooldown;

            updateScore(who, -1);
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
        if (who->nukes > 0 && who->nukeCooldown == 0 && who->heat == 0)
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

    player->health = 6;
    player->momentum = makePoint(0, 0);
    player->heat = 0;
    player->maxCooldown = INITIAL_MAX_COOLDOWN;
    player->heatup = INITIAL_HEATUP;
    player->cooldown = player->maxCooldown;

    player->nukeCooldown = MAX_NUKE_COOLDOWN;
    player->nukes = 3;

    player->kills = 0;
    player->score = 0;

    player->weaponLevel = 0;
    player->spawnProjectile = spawnBullet;

    upgradeWeapon(player);
    upgradeWeapon(player);
    upgradeWeapon(player);
    upgradeWeapon(player);
    upgradeWeapon(player);

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
    if (who->weaponLevel == (MAX_WEAPONS - 1))
        return;

    switch (++who->weaponLevel)
    {
        case 1:
            who->spawnProjectile = spawnDoubleBullet;
            who->maxCooldown = 7;
            who->heatup = 1;
            break;

        case 2:
            who->spawnProjectile = spawnLaser;
            who->maxCooldown = 6;
            who->heatup = 1;
            break;

        case 3:
            who->spawnProjectile = spawnWave;
            who->maxCooldown = 4;
            who->heatup = 1;
            break;

        case 4:
            who->spawnProjectile = spawnBeam;
            who->maxCooldown = 5;
            who->heatup = 23;
            break;
    }
}

void enemyKilled(Player* who, Enemy* enemy, bool collided)
{
    uint16 score = enemy->maxHealth * 13;
    if (collided)
        score >>= 1;

    ++who->kills;
    updateScore(who, score);
}

void updateScore(Player* who, int16 change)
{
    if (change > 0)
        who->score += change;
    else
    {
        if (who->score < -change)
            who->score = 0;
        else
            who->score += change;
    }
}

void drawPlayer(Player* who)
{
    drawSprite(who);
    drawSprite(who->engine);
}
