#include "f18.h"
#include "explosion.h"
#include "chain.h"
#include "lock-on.h"
#include "f18.anim.inl"

static void destroyF18(F18* who)
{
    destroy(who->behavior);
}

static void killF18(F18* who, World* world)
{
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_SMALL, 5);
    alignCenter(e, who);

    addUpdateable(world, e);
}

F18* createF18(int16 x, int16 recess, uint8 health, Behavior* behavior)
{
    Behavior** chain;
    int16 height;

    F18* f18 = new(F18);
    f18->destroy = destroyF18;
    f18->kill = killF18;

    chain = newArray(Behavior*, 2);
    chain[0] = behavior;
    chain[1] = createLockOn(1000);
    f18->behavior = createChainBehavior(chain, 2);

    f18->animation = f18Animation();
    animationBeginning(f18);

    f18->health = health;

    height = entityHeight(f18);
    f18->position.x = x;
    f18->position.y = -height - recess;

    return f18;
}
