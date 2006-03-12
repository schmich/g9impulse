#include "facing.h"
#include "player.h"

#define THRESHOLD 10

static uint8 updateFacing(Updateable* who, World* w)
{
    Facing* f = who->behavior;
    int16 xDiff = spriteCenter(w->player).x - spriteCenter(who).x;
    int16 yDiff = spriteBottom(w->player) - spriteTop(who);

    if (yDiff > 0)
    {
        if (xDiff > THRESHOLD)
            who->currentFrame = 1;
        else if (xDiff < -THRESHOLD)
            who->currentFrame = 2;
        else
            who->currentFrame = 0;
    }
    
    return UPDATE_KEEP;
}

Facing* createFacing(void)
{
    Facing* f = new(Facing);
    f->destroy = nullDestroy;
    f->update = updateFacing;

    return f;
}
