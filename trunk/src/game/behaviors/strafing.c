#include "strafing.h"

static uint8 updateStrafing(Updateable* who, World* world)
{
    Strafing* s = who->behavior;

    if (++s->elapsed == s->frameDelay)
    {
        s->elapsed = 0;

        who->position.x += s->speed; 
        if (s->speed > 0)
        {
            //
            // heading right
            //
            if (spriteLeft(who) > SCREEN_WIDTH)
                return UPDATE_REMOVE;
        }
        else
        {
            //
            // heading left
            //
            if (spriteRight(who) < 0)
                return UPDATE_REMOVE;
        }
    }

    return UPDATE_KEEP;
}

Strafing* createStrafing(int8 speed, uint8 frameDelay)
{
    Strafing* s = new(Strafing);
    s->destroy = nullDestroy;
    s->update = updateStrafing;
    s->speed = speed;
    s->frameDelay = frameDelay;
    s->elapsed = 0;

    return s;
}
