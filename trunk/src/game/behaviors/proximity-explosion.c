#include "explosion.anim.inl"
#include "explosion.h"
#include "proximity-explosion.h"

static uint8 updateProximityExplosion(Updateable* who, World* world)
{
    ProximityExplosion* e = who->behavior;
    Point targetCenter = spriteCenter(e->target);
    Point myCenter = spriteCenter(who);

    int16 xDiff = myCenter.x - targetCenter.x;
    int16 yDiff = myCenter.y - targetCenter.y;
    int16 squareDistance;

    Explosion* explosion;
    Projectile* projectile;

    if (abs(xDiff) < 25 && abs(yDiff) < 25)
    {
        squareDistance = (xDiff * xDiff) + (yDiff * yDiff);
        if (squareDistance < 625)
        {
            explosion = createExplosion(makePoint(0, 0), EXPLOSION_LARGE, 5);

            setSpriteCenter(explosion, myCenter);
            addUpdateable(world, explosion);

            projectile = createProjectile(largeExplosionAnimation(), 0,
                                          createAnimator(5, 1),
                                          2,
                                          makePoint(0, 0),
                                          nullImpact,
                                          false);

            setSpriteCenter(projectile, myCenter);
            addEnemyProjectile(world, projectile);

            return UPDATE_REMOVE;
        }
    }

    return UPDATE_KEEP;
}

ProximityExplosion* createProximityExplosion(Entity* target)
{
    ProximityExplosion* e = new(ProximityExplosion);
    e->destroy = nullDestroy;
    e->update = updateProximityExplosion;
    e->target = target;

    return e;
}
