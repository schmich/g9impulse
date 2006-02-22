#include "player.h"

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

static void updatePlayer(Player* who)
{
    Input* input = getInputStatus();
    uint8 width;
    uint8 height;

    if (input->buttonBPressed)
    {
        //Point tip = entityCenter(player);
        //tip.y = player->position.y;

        //addProjectile(world, createBullet(tip, TEAM_PLAYER));
    }

    if (input->leftPressed)
    {
        player->position.x -= 1;

        if (--player->momentum.x < -(PLAYER_MAX_MOMENTUM_X / 2))
        {
            if (player->momentum.x < -PLAYER_MAX_MOMENTUM_X)
                player->momentum.x = -PLAYER_MAX_MOMENTUM_X;

            rollLeft(player);
        }
    }
    else if (input->rightPressed)
    {
        player->position.x += 1;

        if (++player->momentum.x > (PLAYER_MAX_MOMENTUM_X / 2))
        {
            if (player->momentum.x > PLAYER_MAX_MOMENTUM_X)
                player->momentum.x = PLAYER_MAX_MOMENTUM_X;

            rollRight(player);
        }
    }
    else
    {
        if (player->momentum.x < 0)
            ++player->momentum.x;
        else if (player->momentum.x > 0)
            --player->momentum.x;

        if ((player->momentum.x > -(PLAYER_MAX_MOMENTUM_X / 2)) &&
            (player->momentum.x < (PLAYER_MAX_MOMENTUM_X / 2)))
            noRoll(player);
    }

    if (input->upPressed)
    {
        if (--player->momentum.y < -PLAYER_MAX_MOMENTUM_Y)
            player->momentum.y = -PLAYER_MAX_MOMENTUM_Y;
    }
    else if (input->downPressed)
    {
        if (++player->momentum.y > PLAYER_MAX_MOMENTUM_Y)
            player->momentum.y = PLAYER_MAX_MOMENTUM_Y;
    }
    else
    {
        if (player->momentum.y < 0)
            ++player->momentum.y;
        else if (player->momentum.y > 0)
            --player->momentum.y;
    }

    player->position.x += player->momentum.x / 4;
    player->position.y += player->momentum.y / 3;

    if (player->position.x < 0)
    {
        player->position.x = 0;
    }
    else
    {
        width = entityWidth(player);

        if ((player->position.x + width) > SCREEN_WIDTH)
            player->position.x = SCREEN_WIDTH - width;
    }

    if (player->position.y < 25) // HACK no magic constant (height of border)
    {
        player->position.y = 25;
    }
    else
    {
        height = entityHeight(player);

        if ((player->position.y + height) > (SCREEN_HEIGHT + 5))
            player->position.y = (SCREEN_HEIGHT + 5) - height;
    }
}

Player* createPlayer(Point where)
{
    Player* player = new(Player);
    player->destroy = destroyPlayer;
    player->update = updatePlayer;
    player->position = where;

    player->animation = playerAnimation();
    animationBeginning((Entity*)player);

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
