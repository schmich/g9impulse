#include "f16.h"

static Animation theAnimation;
static bool theInitAnimation = false;

static Animation* f16Animation(void)
{
    if (!theInitAnimation)
    {
        theAnimation.numImages = 2;
        theAnimation.images = newArray(Image, theAnimation.numImages);

        theAnimation.images[0] = makeImage(0x000DA7D0, 15, 49);
        theAnimation.images[1] = makeImage(0x000DAEA1, 15, 49);

        theInitAnimation = true;
    }

    return &theAnimation;
}

static void destroyAnimation(void)
{
    destroyStatic(&theAnimation);
    theInitAnimation = false;
}

static void destroyF16(F16* who)
{
    destroy(who->behavior);
}

F16* createF16(Point where, uint8 health, Behavior* behavior)
{
    F16* f16 = new(F16);
    f16->destroy = destroyF16;
    f16->position = where;
    f16->behavior = behavior;

    f16->animation = f16Animation();
    animationBeginning((Entity*)f16);

    f16->health = health;

    return f16;
}
