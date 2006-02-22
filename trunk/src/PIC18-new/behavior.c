#include "boring.h"

static void updateBoring(Updateable* who)
{
    who->position.y += who->behavior->speed;
    if ((who->position.y + entityHeight(who)) > SCREEN_HEIGHT)
        who->position.y = SCREEN_HEIGHT - entityHeight(who);
}

Boring* createBoring(uint8 speed)
{
    Boring* b = new(Boring);
    b->destroy = nullDestroy();
    b->speed = speed;
    b->update = updateBoring;

    return b;
}
