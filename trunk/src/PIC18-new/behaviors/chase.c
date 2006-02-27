#include "chase.h"

static uint8 updateChase(Updateable* who, World* world)
{
    int16 diff;
    int16 threshold = 5;
    Chase* c = who->behavior;

    who->position.y += c->vertSpeed;

    if (entityTop(world->player) > entityBottom(who))
    {
        diff = entityCenter(world->player).x - entityCenter(who).x;

        if (diff > threshold)
        {
            who->position.x += c->horizSpeed;
            who->currentFrame = 2;
        }
        else if (diff < -threshold)
        {
            who->position.x -= c->horizSpeed;
            who->currentFrame = 1;
        }
        else
        {
            who->currentFrame = 0;
        }
    }

    if (c->vertSpeed > 0)
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

Chase* createChase(int8 vertSpeed, int8 horizSpeed)
{
    Chase* c = new(Chase);
    c->destroy = nullDestroy;
    c->update = updateChase;
    c->vertSpeed = vertSpeed;
    c->horizSpeed = horizSpeed;

    return c;
}
