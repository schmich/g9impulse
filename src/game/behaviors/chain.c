#include "chain.h"

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

ChainBehavior* createChainBehavior(Behavior** behaviors, uint8 numBehaviors)
{
    ChainBehavior* b = new(ChainBehavior);
    b->destroy = destroyChain;
    b->update = updateChain;

    b->behaviors = behaviors;
    b->numBehaviors = numBehaviors;

    return b;
}
