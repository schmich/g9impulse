#include "seek.h"
#include "player.h"

static uint8 updateSeek(Updateable* who, World* world)
{
    Point pc;
    Point mc;
    Seek* s = who->behavior;

    if (!s->locked)
    {
        pc = entityCenter(world->player);
        mc = entityCenter(who);

        if (pc.x > mc.x)
            s->horizSpeed = 1;
        else
            s->horizSpeed = -1;

        s->locked = true;
    }

    who->position.x += s->horizSpeed;
    
    return UPDATE_KEEP;
}

Seek* createSeek(void)
{
    Seek* s = new(Seek);
    s->destroy = nullDestroy;
    s->update = updateSeek;
    s->horizSpeed = 0;
    s->locked = false;

    return s;
}
