#include "health-meter.h"
#include "health.anim.inl"
#include "gpu.h"

static void destroyHealthMeter(HealthMeter* h)
{
    destroy(h->behavior);
}

static uint8 updateHealthMeter(HealthMeter* h, World* w)
{
    if (h->player->health > 1)
    {
        h->currentFrame = 6 - h->player->health;
    }
    else if (h->player->health == 1)
    {
        if (++h->frameDelay == 3)
        {
            h->frameDelay = 0;

            if (h->currentFrame < 5 || h->currentFrame > 10)
            {
                //
                // get into 1-health animation loop
                //
                h->currentFrame = 5;
            }
            else if (++h->currentFrame == 11)
            {
                //
                // reset to beginning of 1-health animation loop
                //
                h->currentFrame = 5;
            }
        }
    }
    else
    {
        //
        // health == 0
        //
        h->currentFrame = 11;
    }
    
    return UPDATE_KEEP;
}

HealthMeter* createHealthMeter(Player* who, Point where)
{
    HealthMeter* h = new(HealthMeter);
    h->destroy = destroyHealthMeter;
    h->behavior = createBehavior(updateHealthMeter);
    h->animation = healthAnimation();
    h->position = where;
    h->frameDelay = 0;
    h->player = who;
    h->currentFrame = 0;

    return h;
}
