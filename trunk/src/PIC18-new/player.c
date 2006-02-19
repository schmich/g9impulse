#include "player.h"

static Animation theAnimation;
static bool theInitAnimation = false;

static Animation* playerAnimation(void)
{
    if (!theInitAnimation)
    {
        theAnimation.numImages = 3;
        theAnimation.images = (Image*)alloc(sizeof(Image) * theAnimation.numImages);

        theAnimation.images[0] = makeImage(0x000D7A20, 16, 28);   // centered
        theAnimation.images[1] = makeImage(0x000D8BA0, 16, 28);   // right
        theAnimation.images[2] = makeImage(0x000D9D20, 16, 28);   // left

        theInitAnimation = true;
    }

    return &theAnimation;
}

static void destroyAnimation(void)
{
    free(theAnimation.images);
    theInitAnimation = false;
}

Player* createPlayer(Point where)
{
    Player* player = (Player*)alloc(sizeof(Player));
    player->position = where;

    player->animation = playerAnimation();
    animationBeginning((Entity*)player);

    player->health = 5;
    player->momentum.x = 0;
    player->momentum.y = 0;
    player->heat = 0;

    return player;
}

void destroyPlayer(Player* who)
{
    free(who);
    destroyAnimation();
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
