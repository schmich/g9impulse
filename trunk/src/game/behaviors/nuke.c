#include "nuke.h"
#include "projectile.h"
#include "animation.h"
#include "animator.h"
#include "spawner.h"
#include "world.h"
#include "explosion.anim.inl"

#define SPACE_X (int16)25
#define SPACE_Y (int16)65

typedef Animation* (*AnimationFn)(void);

static Projectile* createNukeExplosion(Point where, uint8 size)
{
    static const rom AnimationFn animations[] =
    {
        tinyExplosionAnimation,
        smallExplosionAnimation,
        mediumExplosionAnimation,
        largeExplosionAnimation
    };

    return createProjectile(animations[size](), 0,
                            createAnimator(5, 1),
                            100,
                            where,
                            nullImpact,
                            true);

}

static void addNuke(World* world, Point where, uint8 delay)
{
    addUpdateable(
        world,
        createSpawner(
            createNukeExplosion(where, 3),
            delay,
            SPAWNER_PLAYER_PROJECTILE
        )
    );
}

static uint8 updateNuke(Updateable* who, World* w)
{
    Point where;
    Nuke* nuke = who->behavior;

    if (who->position.y < nuke->yDetonate)
    {
        where = spriteCenter(who);
        where.x -= 20;
        where.y -= 40;

        addNuke(w, where, 0);

        where.x -= SPACE_X;
        addNuke(w, where, 20);

        where.x += 2 * SPACE_X;
        addNuke(w, where, 20);

        where.x -= SPACE_X;
        where.y -= SPACE_Y;
        addNuke(w, where, 20);

        where.y += 2 * SPACE_Y;
        addNuke(w, where, 20);

        return UPDATE_REMOVE;
    }

    return UPDATE_KEEP;
}

Nuke* createNuke(int16 yDetonate)
{
    Nuke* n = new(Nuke);
    n->destroy = nullDestroy;
    n->update = updateNuke;
    n->yDetonate = yDetonate;

    return n;
}

/*static void dropBombs(Player* who, World* world)
{
    uint16 i;
    Projectile* explosion;
    Point where;

    for (i = 0; i < 8; ++i)
    {
        where.x = random(who->position.x - 10, who->position.x + 10);
        where.y = who->position.y - 60 * (i + 1);

        explosion = createBombExplosion(where, 3);
        addUpdateable(world,
                      createSpawner(explosion,
                                    random(0, 25),
                                    SPAWNER_PLAYER_PROJECTILE));
    }
}*/
