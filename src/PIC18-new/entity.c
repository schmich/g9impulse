#include "entity.h"

void kill(Entity* who, World* world)
{
    who->kill(who, world);
}

bool damage(Entity* who, uint8 damage)
{
    if (damage >= who->health)
    {
        who->health = 0;
        return true;
    }
    else
    {
        who->health -= damage;
        return false;
    }
}

bool dead(Entity* who)
{
    return who->health == 0;
}

void fire(Entity* e, World* w)
{
    e->fire(e, w);
}
