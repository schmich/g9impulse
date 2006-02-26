#include "f16.h"
#include "explosion.h"
#include "plasma.h"
#include "behavior.h"
#include "f16.anim.inl"

static void destroyF16(F16* who)
{
    destroy(who->behavior);
}

static void killF16(F16* who, World* world)
{
    Explosion* e = createExplosion(makePoint(0, 0), EXPLOSION_SMALL, 5);
    alignCenter(e, who);

    addUpdateable(world, e);
}

static uint8 shootF16(F16* who, World* world)
{
    Plasma* p;

    if (entityBottom(who) > 0)
    {
        if (rand() < 300)
        {
            p = createPlasma(makePoint(0, 0), 7);
            alignCenterBottom(p, who);
            p->position.y += entityHeight(p);

            addEnemyProjectile(world, p);
        }
    }

    return UPDATE_KEEP;
}

F16* createF16(int8 x, uint8 recess, uint8 health, Behavior* behavior)
{
    Behavior** chain;
    int16 height;

    F16* f16 = new(F16);
    f16->destroy = destroyF16;
    f16->kill = killF16;

    chain = newArray(Behavior*, 2);
    chain[0] = behavior;
    chain[1] = createBehavior(shootF16);
    f16->behavior = createChainBehavior(chain, 2);

    f16->animation = f16Animation();
    animationBeginning(f16);

    f16->health = health;

    height = entityHeight(f16);
    f16->position.x = x;
    f16->position.y = -height - recess;

    return f16;
}
