#ifndef POWERUP_H
#define POWERUP_H

#include "common.h"
#include "artifact.h"

void powerupHealth(Artifact* a, Player* p, World* w);
void powerupWeapon(Artifact* a, Player* p, World* w);
void powerupNuke(Artifact* a, Player* p, World* w);

#endif // POWERUP_H
