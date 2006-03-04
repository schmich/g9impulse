#include "boring.h"

static uint8 updateBoring(Updateable* who, World* world)
{
    Boring* b = who->behavior;

    if (++b->counter == b->vertSpeed.denominator)
    {
        who->position.y += b->vertSpeed.numerator;
        b->counter = 0;
    }

    if (b->vertSpeed.numerator > 0)
    {
        //
        // heading downscreen
        //
        if (spriteTop(who) > SCREEN_HEIGHT)
            return UPDATE_REMOVE;
    }
    else
    {
        //
        // heading upscreen
        //
        if (spriteBottom(who) < 0)
            return UPDATE_REMOVE;
    }

    return UPDATE_KEEP;
}

Boring* createBoring(Fraction vertSpeed)
{
    Boring* b = new(Boring);
    b->destroy = nullDestroy;
    b->update = updateBoring;
    b->vertSpeed = vertSpeed;
    b->counter = 0;

    return b;
}
