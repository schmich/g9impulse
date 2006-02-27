#include "random-shoot.h"
#include "plasma.h"

static uint8 updateRandomShoot(Updateable* who, World* world)
{
    Plasma* p;
    RandomShoot* r = who->behavior;

    if (entityBottom(who) > 0)
    {
        if (rand() < r->frequency)
        {
            p = createPlasma(makePoint(0, 0), 5);
            alignCenterBottom(p, who);
            p->position.y += entityHeight(p);

            addEnemyProjectile(world, p);
        }
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
