#include "powerup.h"

void powerupHealth(Artifact* a, Player* p, World* w)
{
    if (p->health < 6)
        p->health += 1;

    updateScore(p, p->health * 25);

    ++p->stats->artifactsCollected;
}

void powerupWeapon(Artifact* a, Player* p, World* w)
{
    updateScore(p, 300);
    upgradeWeapon(p);
    ++p->stats->artifactsCollected;
}

void powerupNuke(Artifact* a, Player* p, World* w)
{
    updateScore(p, 250);
    ++p->nukes;
    ++p->stats->artifactsCollected;
}
