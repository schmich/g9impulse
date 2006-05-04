#include "powerup.h"

void powerupHealth(Artifact* a, Player* p, World* w)
{
    if (p->health < 6)
    {
        updateScore(p, 25);
        p->health += 1;
    }
    else
        updateScore(p, 100);

    ++p->stats->artifactsCollected;
}

void powerupWeapon(Artifact* a, Player* p, World* w)
{
    updateScore(p, 200);
    upgradeWeapon(p);
    ++p->stats->artifactsCollected;
}

void powerupNuke(Artifact* a, Player* p, World* w)
{
    updateScore(p, 50);
    ++p->nukes;
    ++p->stats->artifactsCollected;
}
