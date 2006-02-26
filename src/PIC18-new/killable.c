#include "killable.h"

void kill(Killable* who, World* world)
{
    who->kill(who, world);
}

bool damage(Killable* who, uint8 damage)
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
