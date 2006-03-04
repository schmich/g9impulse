#include "animator.h"

static uint8 updateAnimator(Updateable* who, World* world)
{
    Animator* b = who->behavior;

    if (++b->elapsed == b->frames)
    {
        b->elapsed = 0;
        if (animationNext(who))
            return UPDATE_REMOVE;
    }

    return UPDATE_KEEP;
}

Animator* createAnimator(uint8 frames)
{
    Animator* a = new(Animator);
    a->destroy = nullDestroy;
    a->update = updateAnimator;
    a->frames = frames;
    a->elapsed = 0;

    return a;
}
