#include "boring.h"

static uint8 updateBoring(Updateable* who, World* world)
{
    Boring* b = who->behavior;
    who->position.y += b->vertSpeed;

    if (b->vertSpeed > 0)
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

Boring* createBoring(int8 vertSpeed)
{
    Boring* b = new(Boring);
    b->destroy = nullDestroy;
    b->update = updateBoring;
    b->vertSpeed = vertSpeed;

    return b;
}
