#include "lock-on.h"
#include "enemy.h"
#include "player.h"

static uint8 updateLockOn(Enemy* who, World* world)
{
    int16 diff;
    int16 threshold = 10;
    LockOn* lo = who->behavior;

    diff = spriteCenter(world->player).x - spriteCenter(who).x;
    if ((diff <= threshold) && (diff >= -threshold))
    {
        if (rand() < lo->frequency)
            fire(who, world);
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
