#include "updateable.h"

void update(Updateable* who)
{
    who->behavior->update(who);
}

Behavior* createBehavior(UpdateFn update)
{
    Behavior* b = new(Behavior);
    b->destroy = nullDestroy();
    b->update = update;

    return b;
}
