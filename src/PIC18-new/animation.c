#include "animation.h"

static void destroyAnimation(Animation* a)
{
    free(a->images);
}

Animation* createAnimation(void)
{
    Animation* a = new(Animation);
    a->destroy = destroyAnimation;
    a->images = NULL;
    a->numImages = 0;

    return a;
}
