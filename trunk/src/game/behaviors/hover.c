#include "hover.h"

static uint8 updateHover(Updateable* who, World* world)
{
    Hover* h = who->behavior;

    if (h->retreat)
    {
        --who->position.y;
        if (spriteBottom(who) < 0)
            return UPDATE_REMOVE;
    }
    else
    {
        if (spriteBottom(who) < h->yMin)
        {
            ++who->position.y; 
        }
        else
        {
            if (--h->duration == 0)
                h->retreat = true;
        }
    }

    return UPDATE_KEEP;
}

Hover* createHover(int16 yMin, uint16 duration)
{
    Hover* h = new(Hover);
    h->destroy = nullDestroy;
    h->update = updateHover;
    h->yMin = yMin;
    h->duration = duration;
    h->retreat = false;

    return h;
}
