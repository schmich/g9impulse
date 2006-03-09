#include "player.h"
#include "input.h"
#include "explosion.h"
#include "world.h"
#include "projectile.h"
#include "boring.h"
#include "chain.h"
#include "player.anim.inl"
#include "bullet.anim.inl"

#define MAX_MOMENTUM_Y 13
#define MAX_MOMENTUM_X 10
#define MAX_COOLDOWN 7

static void destroyPlayer(Player* p)
{
    destroy(p->behavior);
}

static void killPlayer(Player* who, World* world)
{
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_LARGE, 7);
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

static void spawnProjectilePlayer(Entity* player, World* world, Projectile** p)
{
    *p = createProjectile(bulletAnimation(), 0,
                          createBoring(makeWhole(-7)),
                          1,
                          makePoint(0, 0),
                          impactProjectile);
}

static uint8 updatePlayer(Player* who, World* world)
{
    Projectile* bullet;
    Input* input = getInputStatus();
    uint8 width;
    uint8 height;

    if (getInputEvent()->buttonBPressed)
    {
        if (who->heat < 6)
        {
            fire(who, world);

            ++who->heat;
            who->cooldown = MAX_COOLDOWN;
        }
    }
    else
    {
        if (who->heat > 0)
        {
            if (--who->cooldown == 0)
            {
                --who->heat;
                who->cooldown = MAX_COOLDOWN;
            }
        }
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

    player->health = 6;
    player->momentum.x = 0;
    player->momentum.y = 0;
    player->heat = 0;
    player->cooldown = MAX_COOLDOWN;

    player->spawnProjectile = spawnProjectilePlayer;

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
