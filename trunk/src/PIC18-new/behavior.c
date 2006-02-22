#include "behavior.h"

static void updateBoring(Updateable* who, World* world)
{
    Boring* behavior = (Boring*)who->behavior;
    who->position.y += behavior->speed;

    if ((who->position.y + entityHeight(who)) > SCREEN_HEIGHT)
        who->position.y = SCREEN_HEIGHT - entityHeight(who);
}

Boring* createBoring(int8 speed)
{
    Boring* b = new(Boring);
    b->destroy = nullDestroy;
    b->speed = speed;
    b->update = updateBoring;

    return b;
}
