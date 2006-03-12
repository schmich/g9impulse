#include "engine.h"
#include "engine.anim.inl"
#include "player.h"

#define FRAME_DELAY 5

static void destroyEngine(Engine* e)
{
    destroy(e->behavior);
}

static uint8 updateEngine(Engine* e, World* w)
{
    int16 x = e->player->position.x;
    int16 y = e->player->position.y;

    if (y > e->yPrevious)
    {
        //
        // going backwards
        //
 
        if (++e->frameDelay == FRAME_DELAY)
        {
            if (e->currentFrame == 0)
                e->currentFrame = 1;
            else
                e->currentFrame = 0;

            e->frameDelay = 0;
        }
    }
    else
    {
        //
        // going forwards or not moving
        //

        if (++e->frameDelay == FRAME_DELAY)
        {
            if (e->currentFrame == 3)
                e->currentFrame = 4;
            else
                e->currentFrame = 3;

            e->frameDelay = 0;
        }
    }

    e->position.x = x + 6;
    e->position.y = y + 23;

    e->yPrevious = y;

    return UPDATE_KEEP;
}

Engine* createEngine(Player* who)
{
    Engine* e = new(Engine);
    e->destroy = destroyEngine;
    e->behavior = createBehavior(updateEngine);
    e->animation = engineAnimation();
    e->player = who;
    e->position = makePoint(who->position.x + 6, who->position.y + 23);

    e->frameDelay = 0;
    e->currentFrame = 2;

    return e;
}
