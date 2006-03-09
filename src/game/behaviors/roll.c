#include "roll.h"

static uint8 updateRoll(Updateable* who, World* world)
{
    int16 x = who->position.x;
    Roll* r = who->behavior;

    if (x < r->xPrevious)
        who->currentFrame = 1;
    else if (x > r->xPrevious)
        who->currentFrame = 2;
    else
        who->currentFrame = 0;
    
    r->xPrevious = x;

    return UPDATE_KEEP;
}

Roll* createRoll(int16 xInit)
{
    Roll* r = new(Roll);
    r->destroy = nullDestroy;
    r->update = updateRoll;
    r->xPrevious = xInit;

    return r;
}
