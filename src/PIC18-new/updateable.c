#include "updateable.h"

void update(Updateable* who, World* world)
{
    who->behavior->update(who, world);
}

Behavior* createBehavior(UpdateFn update)
{
    Behavior* b = new(Behavior);
    b->destroy = nullDestroy;
    b->update = update;

    return b;
}
