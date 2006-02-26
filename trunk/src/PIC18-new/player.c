#include "player.h"
#include "input.h"
#include "bullet.h"

#define PLAYER_MAX_MOMENTUM_Y 13
#define PLAYER_MAX_MOMENTUM_X 10

static Animation theAnimation;
static bool theInitAnimation = false;

static Animation* playerAnimation(void)
{
    if (!theInitAnimation)
    {
        theAnimation.numImages = 3;
        theAnimation.images = newArray(Image, theAnimation.numImages);

        theAnimation.images[0] = makeImage(0x000D7A20, 16, 28);   // centered
        theAnimation.images[1] = makeImage(0x000D8BA0, 16, 28);   // right
        theAnimation.images[2] = makeImage(0x000D9D20, 16, 28);   // left

        theInitAnimation = true;
    }

    return &theAnimation;
}

static void destroyAnimation(void)
{
    destroyStatic(&theAnimation);
    theInitAnimation = false;
}

static void destroyPlayer(Player* who)
{
    destroyAnimation();
}

static uint8 updatePlayer(Player* who, World* world)
{
    Input* input = getInputStatus();
    uint8 width;
    uint8 height;

    if (getInputEvent()->buttonBPressed)
    {
        Point tip = entityCenter(who);
        tip.y = who->position.y;

        //
        // HACK fudge factor, center bullet
        //
        tip.x--;

        addPlayerProjectile(world, createBullet(tip, -7));
    }

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
    Player* player = new(Player);
    player->destroy = destroyPlayer;
    player->behavior = createBehavior(updatePlayer);
    player->position = where;

    player->animation = playerAnimation();
    animationBeginning(player);

    player->health = 5;
    player->momentum.x = 0;
    player->momentum.y = 0;
    player->heat = 0;

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
