#include "updateable.h"

uint8 update(Updateable* who, World* world)
{
    return who->behavior->update(who, world);
}

uint8 nullUpdate(Updateable* who, World* world)
{
    return UPDATE_KEEP;
}

Behavior* createBehavior(UpdateFn update)
{
    Behavior* b = new(Behavior);
    b->destroy = nullDestroy;
    b->update = update;

    return b;
}
