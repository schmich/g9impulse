#include "health-meter.h"
#include "health.anim.inl"
#include "gpu.h"

static void destroyHealthMeter(HealthMeter* h)
{
    destroy(h->behavior);
}

static uint8 updateHealthMeter(HealthMeter* h, World* w)
{
    if (h->player->health == 1)
    {
        if (++h->frameDelay == 10)
        {
            if (h->currentFrame == 6)
                h->currentFrame = 5;
            else
                h->currentFrame = 6;

            h->frameDelay = 0;
        }
    }
    else
    {
        if (h->player->health <= 6)
            h->currentFrame = 6 - h->player->health;
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
