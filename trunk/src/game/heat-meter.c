#include "heat-meter.h"
#include "heat.anim.inl"
#include "gpu.h"

static void destroyHeatMeter(HeatMeter* h)
{
    destroy(h->behavior);
}

static uint8 updateHeatMeter(HeatMeter* h, World* w)
{
    if (h->player->heat >= 6)
    {
        if (++h->frameDelay == 3)
        {
            if (h->currentFrame == 5)
                h->currentFrame = 6;
            else
                h->currentFrame = 5;

            h->frameDelay = 0;
        }
    }
    else
    {
        if (h->player->heat <= 6)
            h->currentFrame = h->player->heat;
    }

    return UPDATE_KEEP;
}

HeatMeter* createHeatMeter(Player* who, Point where)
{
    HeatMeter* h = new(HeatMeter);
    h->destroy = destroyHeatMeter;
    h->behavior = createBehavior(updateHeatMeter);
    h->animation = heatAnimation();
    h->position = where;
    h->frameDelay = 0;
    h->player = who;
    h->currentFrame = 0;

    return h;
}
