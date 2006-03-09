#include "enemy.h"

static void destroyEnemy(Enemy* e)
{
    destroy(e->behavior);
}

static void fireEnemy(Entity* e, World* w)
{
    Projectile* p;
    e->spawnProjectile(e, w, &p);

    if (p)
    {
        setSpriteCenterBottom(p, spriteCenterBottom(e));
        p->position.y += spriteHeight(p);

        addEnemyProjectile(w, p);
    }
}

Enemy* createEnemy(Animation* anim,
                   uint8 initFrame, 
                   Behavior* behavior,
                   uint8 health,
                   Point where,
                   SpawnFn onProjectileSpawn,
                   KillFn onKill)
{
    Enemy* e = new(Enemy);
    e->destroy = destroyEnemy;
    e->behavior = behavior;
    e->position = where;
    e->health = health;
    e->kill = onKill;
    e->fire = fireEnemy;
    e->animation = anim;
    e->currentFrame = initFrame;
    e->spawnProjectile = onProjectileSpawn;

    return e;
}
