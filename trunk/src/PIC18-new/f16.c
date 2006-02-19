#include "f16.h"

static Animation theAnimation;
static bool theInitAnimation = false;

static Animation* f16Animation(void)
{
    if (!theInitAnimation)
    {
        theAnimation.numImages = 2;
        theAnimation.images = (Image*)alloc(sizeof(Image) * theAnimation.numImages);

        theAnimation.images[0] = makeImage(0x000DA7D0, 15, 49);
        theAnimation.images[1] = makeImage(0x000DAEA1, 15, 49);

        theInitAnimation = true;
    }

    return &theAnimation;
}

static void destroyAnimation(void)
{
    free(theAnimation.images);
    theInitAnimation = false;
}

F16* createF16(Point where, uint8 health, Behavior behavior)
{
    F16* f16 = (F16*)alloc(sizeof(F16));
    f16->type = 0;
    f16->position = where;
    f16->think = behavior;

    f16->animation = f16Animation();
    animationBeginning((Entity*)f16);

    f16->health = health;

    return f16;
}

void destroyF16(F16* who)
{
    free(who);
}

void boringF16(Enemy* self)
{
    self->position.y += 2;
    if ((self->position.y + entityHeight(self)) > SCREEN_HEIGHT)
        self->position.y = SCREEN_HEIGHT - entityHeight(self);
}
