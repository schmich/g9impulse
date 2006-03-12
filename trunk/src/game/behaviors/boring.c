#include "boring.h"

static uint8 updateBoring(Updateable* who, World* world)
{
    Boring* b = who->behavior;

    if (++b->elapsed == b->frameDelay)
    {
        b->elapsed = 0;

        who->position.y += b->speed; 
        if (b->speed > 0)
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
    }

    return UPDATE_KEEP;
}

Boring* createBoring(int8 speed, uint8 frameDelay)
{
    Boring* b = new(Boring);
    b->destroy = nullDestroy;
    b->update = updateBoring;
    b->speed = speed;
    b->frameDelay = frameDelay;
    b->elapsed = 0;

    return b;
}
