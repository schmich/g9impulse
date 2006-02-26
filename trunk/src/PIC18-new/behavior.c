#include "behavior.h"

static uint8 updateBoring(Updateable* who, World* world)
{
    Boring* b = who->behavior;
    who->position.y += b->speed;

    if (b->speed > 0)
    {
        //
        // heading downscreen
        //
        if (entityTop(who) > SCREEN_HEIGHT)
            return UPDATE_REMOVE;
    }
    else
    {
        //
        // heading upscreen
        //
        if (entityBottom(who) < 0)
            return UPDATE_REMOVE;
    }

    return UPDATE_KEEP;
}

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

static uint8 updateChain(Updateable* who, World* world)
{
    uint8 i;
    ChainBehavior* chain = who->behavior;
    
    for (i = 0; i < chain->numBehaviors; ++i)
    {
        who->behavior = chain->behaviors[i];

        if (chain->behaviors[i]->update(who, world) == UPDATE_REMOVE)
        {
            who->behavior = chain;
            return UPDATE_REMOVE;
        }
    }

    who->behavior = chain;
    return UPDATE_KEEP;
}

static void destroyChain(ChainBehavior* b)
{
    uint8 i;

    for (i = 0; i < b->numBehaviors; ++i)
        destroy(b->behaviors[i]);

    free(b->behaviors);
}

Boring* createBoring(int8 speed)
{
    Boring* b = new(Boring);
    b->destroy = nullDestroy;
    b->update = updateBoring;
    b->speed = speed;

    return b;
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

ChainBehavior* createChainBehavior(Behavior** behaviors, uint8 numBehaviors)
{
    ChainBehavior* b = new(ChainBehavior);
    b->destroy = destroyChain;
    b->update = updateChain;

    b->behaviors = behaviors;
    b->numBehaviors = numBehaviors;

    return b;
}
