#include "updateable.h"

static void destroyUpdateable(Updateable* who)
{
    destroy(who->behavior);
}

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

Updateable* createUpdateable(Animation* anim,
                             uint8 initFrame,
                             Behavior* behavior,
                             Point where)
{
    Updateable* u = new(Updateable);
    u->destroy = destroyUpdateable;
    u->animation = anim;
    u->currentFrame = initFrame;
    u->behavior = behavior;
    u->position = where;

    return u;
}
