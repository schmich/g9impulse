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
    Point center;
    Point where;
    Nuke* nuke = who->behavior;

    if (who->position.y < nuke->yDetonate)
    {
        center = spriteCenter(who);
        center.x -= 20;
        center.y -= 40;

        where = center;
        addNuke(w, where, 0);

        where = center;
        where.x -= SPACE_X;
        addNuke(w, where, 20);

        where = center;
        where.x += SPACE_X;
        addNuke(w, where, 20);

        where = center;
        where.y -= SPACE_Y;
        addNuke(w, where, 20);

        where = center;
        where.y += SPACE_Y;
        addNuke(w, where, 20);

        where = center;
        where.y += (SPACE_Y * 7) / 10;
        where.x += (SPACE_X * 7) / 10;
        addNuke(w, where, 20);

        where = center;
        where.y -= (SPACE_Y * 7) / 10;
        where.x -= (SPACE_X * 7) / 10;
        addNuke(w, where, 20);

        where = center;
        where.y += (SPACE_Y * 7) / 10;
        where.x -= (SPACE_X * 7) / 10;
        addNuke(w, where, 20);

        where = center;
        where.y -= (SPACE_Y * 7) / 10;
        where.x += (SPACE_X * 7) / 10;
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
