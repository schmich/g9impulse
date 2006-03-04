#include "random-shoot.h"
#include "player.h"
#include "enemy.h"

static uint8 updateRandomShoot(Enemy* who, World* world)
{
    RandomShoot* r = who->behavior;

    if (spriteBottom(who) > 0)
    {
        if (rand() < r->frequency)
            fire(who, world);
    }

    return UPDATE_KEEP;
}

RandomShoot* createRandomShoot(uint16 frequency)
{
    RandomShoot* r = new(RandomShoot);
    r->destroy = nullDestroy;
    r->update = updateRandomShoot;
    r->frequency = frequency;

    return r;
}
