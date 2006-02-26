#include "f16.h"
#include "explosion.h"
#include "plasma.h"
#include "behavior.h"

static Animation theAnimation;
static bool theInitAnimation = false;

static Animation* f16Animation(void)
{
    if (!theInitAnimation)
    {
        theAnimation.numImages = 2;
        theAnimation.images = newArray(Image, theAnimation.numImages);

        theAnimation.images[0] = makeImage(0x000DA7D0, 15, 49);
        theAnimation.images[1] = makeImage(0x000DAEA1, 15, 49);

        theInitAnimation = true;
    }

    return &theAnimation;
}

static void destroyAnimation(void)
{
    destroyStatic(&theAnimation);
    theInitAnimation = false;
}

static void destroyF16(F16* who)
{
    destroy(who->behavior);
}

static void killF16(F16* who, World* world)
{
    Explosion* ex = createExplosion(makePoint(0, 0), EXPLOSION_SMALL, 5);
    alignCenter(ex, who);

    addUpdateable(world, ex);
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
