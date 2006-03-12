#include "powerup.h"

void powerupHealth(Artifact* a, Player* p, World* w)
{
    if (p->health < 6)
        p->health += 1;
    else
        updateScore(p, 100);
}

void powerupWeapon(Artifact* a, Player* p, World* w)
{
    upgradeWeapon(p);
}
