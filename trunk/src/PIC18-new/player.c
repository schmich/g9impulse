#include "player.h"
#include "input.h"
#include "explosion.h"
#include "world.h"
#include "projectile.h"
#include "boring.h"
#include "input-shoot.h"
#include "player.anim.inl"
#include "bullet.anim.inl"

#define PLAYER_MAX_MOMENTUM_Y 13
#define PLAYER_MAX_MOMENTUM_X 10

static void destroyPlayer(Player* p)
{
    destroy(p->behavior);
    destroy(p->weapon);
}

static void killPlayer(Player* who, World* world)
{
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_LARGE, 7);
    alignCenter(e, who);

    addUpdateable(world, e);
}

static void impactProjectile(Projectile* p, Entity* e, World* world)
{
    addUpdateable(world, createExplosion(p->position, EXPLOSION_TINY, 10));
}

static uint8 updatePlayer(Player* who, World* world)
{
    Projectile* bullet;
    Input* input = getInputStatus();
    uint8 width;
    uint8 height;

    if (input->leftPressed)
    {
        who->position.x -= 1;

        if (--who->momentum.x < -(PLAYER_MAX_MOMENTUM_X / 2))
        {
            if (who->momentum.x < -PLAYER_MAX_MOMENTUM_X)
                who->momentum.x = -PLAYER_MAX_MOMENTUM_X;

            rollLeft(who);
        }
    }
    else if (input->rightPressed)
    {
        who->position.x += 1;

        if (++who->momentum.x > (PLAYER_MAX_MOMENTUM_X / 2))
        {
            if (who->momentum.x > PLAYER_MAX_MOMENTUM_X)
                who->momentum.x = PLAYER_MAX_MOMENTUM_X;

            rollRight(who);
        }
    }
    else
    {
        if (who->momentum.x < 0)
            ++who->momentum.x;
        else if (who->momentum.x > 0)
            --who->momentum.x;

        if ((who->momentum.x > -(PLAYER_MAX_MOMENTUM_X / 2)) &&
            (who->momentum.x < (PLAYER_MAX_MOMENTUM_X / 2)))
            noRoll(who);
    }

    if (input->upPressed)
    {
        if (--who->momentum.y < -PLAYER_MAX_MOMENTUM_Y)
            who->momentum.y = -PLAYER_MAX_MOMENTUM_Y;
    }
    else if (input->downPressed)
    {
        if (++who->momentum.y > PLAYER_MAX_MOMENTUM_Y)
            who->momentum.y = PLAYER_MAX_MOMENTUM_Y;
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
        width = entityWidth(who);

        if ((who->position.x + width) > SCREEN_WIDTH)
            who->position.x = SCREEN_WIDTH - width;
    }

    if (who->position.y < 25) // HACK no magic constant (height of border)
    {
        who->position.y = 25;
    }
    else
    {
        height = entityHeight(who);

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

    bs = newArray(Behavior*, 2);
    bs[0] = createBehavior(updatePlayer);
    bs[1] = createInputShoot();
    player->behavior = createChainBehavior(bs, 2);

    player->animation = playerAnimation();
    animationBeginning(player);

    player->health = 6;
    player->momentum.x = 0;
    player->momentum.y = 0;
    player->heat = 0;

    player->weapon = createProjectile(bulletAnimation(), 0,
                                      createBoring(makeWhole(-7)),
                                      1,
                                      makePoint(0, 0),
                                      impactProjectile);
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
