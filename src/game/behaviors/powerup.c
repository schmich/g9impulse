#include "powerup.h"

void addHealth(Artifact* a, Player* p, World* w)
{
    if (p->health < 6)
        p->health += 1;
}
