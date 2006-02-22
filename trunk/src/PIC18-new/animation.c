#include "animation.h"

static void destroyAnimation(Animation* a)
{
    free(a->images);
}

Animation* createAnimation()
{
    Animation* a = new(Animation);
    a->destroy = destroyAnimation;

    return a;
}
