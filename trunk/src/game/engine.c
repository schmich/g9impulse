#include "engine.h"
#include "engine.anim.inl"
#include "player.h"

#define FRAME_DELAY 3

static void destroyEngine(Engine* e)
{
    destroy(e->behavior);
}

static uint8 updateEngine(Engine* e, World* w)
{
    int16 x = e->player->position.x;
    int16 y = e->player->position.y;

    if (e->player->boost)
    {
        e->animation = e->boost;
        if (++e->frameDelay == FRAME_DELAY)
        {
            animationNext(e);
            e->frameDelay = 0;
        }

        e->position.x = x + 4;
        e->position.y = y + 22;
    }
    else
    {
        if (y > e->yPrevious)
        {
            //
            // going backwards
            //
            
            if (e->animation == e->reverse)
            {
                if (++e->frameDelay == FRAME_DELAY)
                {
                    animationNext(e);
                    e->frameDelay = 0;
                }
            }
            else
            {
                e->animation = e->reverse;
                e->currentFrame = 0;
            }
        }
        else
        {
            //
            // going forwards or not moving
            //

            if (e->animation == e->thrust)
            {
                if (++e->frameDelay == FRAME_DELAY)
                {
                    animationNext(e);
                    e->frameDelay = 0;
                }
            }
            else
            {
                e->animation = e->thrust;
                e->currentFrame = 0;
            }
        }

        e->position.x = x + 6;
        e->position.y = y + 23;
    }

    e->yPrevious = y;

    return UPDATE_KEEP;
}

Engine* createEngine(Player* who)
{
    Engine* e = new(Engine);
    e->destroy = destroyEngine;
    e->behavior = createBehavior(updateEngine);
    e->player = who;
    e->position = makePoint(who->position.x + 6, who->position.y + 23);

    e->idle    = engineIdleAnimation();
    e->thrust  = engineThrustAnimation();
    e->reverse = engineReverseAnimation();
    e->boost   = engineBoostAnimation();
    e->animation = e->idle;

    e->frameDelay = 0;
    e->currentFrame = 0;

    return e;
}
