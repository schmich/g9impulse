#include "enemy.h"

static void destroyEnemy(Enemy* e)
{
    destroy(e->behavior);
    destroy(e->weapon);
}

Enemy* createEnemy(Animation* anim,
                   uint8 initFrame, 
                   Behavior* behavior,
                   uint8 health,
                   Point where,
                   Projectile* weapon,
                   KillFn onKill)
{
    Enemy* e = new(Enemy);
    e->destroy = destroyEnemy;
    e->behavior = behavior;
    e->position = where;
    e->health = health;
    e->kill = onKill;
    e->animation = anim;
    e->currentFrame = initFrame;
    e->weapon = weapon;

    return e;
}

void fire(Enemy* e, World* w)
{
    Projectile* p = new(Projectile);
    *p = *e->weapon;

    alignCenterBottom(p, e);
    p->position.y += spriteHeight(p);

    addEnemyProjectile(w, p);
}
