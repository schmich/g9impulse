#include "animator.h"

static uint8 updateAnimator(Updateable* who, World* world)
{
    Animator* b = who->behavior;

    if (++b->elapsed == b->frameDelay)
    {
        b->elapsed = 0;
        if (animationNext(who))
        {
            if (b->loops != FOREVER)
            {
                if (--b->loops == 0)
                    return UPDATE_REMOVE;
            }
        }
    }

    return UPDATE_KEEP;
}

Animator* createAnimator(uint8 frameDelay, uint8 loopCount)
{
    Animator* a = new(Animator);
    a->destroy = nullDestroy;
    a->update = updateAnimator;
    a->frameDelay = frameDelay;
    a->elapsed = 0;
    a->loops = loopCount;

    return a;
}
