#include "lock-on.h"
#include "fireball.h"

static uint8 updateLockOn(Updateable* who, World* world)
{
    Fireball* f;
    int16 diff;
    int16 threshold = 10;
    LockOn* lo = who->behavior;

    diff = entityCenter(world->player).x - entityCenter(who).x;
    if ((diff <= threshold) && (diff >= -threshold))
    {
        if (rand() < lo->frequency)
        {
            f = createFireball(makePoint(0, 0), 5);
            alignCenterBottom(f, who);
            f->position.y += entityHeight(f);

            addEnemyProjectile(world, f);
        }
    }

    return UPDATE_KEEP;
}

LockOn* createLockOn(uint16 frequency)
{
    LockOn* lo = new(LockOn);
    lo->destroy = nullDestroy;
    lo->update = updateLockOn;
    lo->frequency = frequency;

    return lo;
}
