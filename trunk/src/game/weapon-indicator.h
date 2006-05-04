#ifndef WEAPONINDICATOR_H
#define WEAPONINDICATOR_H

#include "common.h"
#include "sprite.h"
#include "player.h"

typedef struct WeaponIndicator
{
    SPRITE_BASE;
    Player* player;
} WeaponIndicator;

WeaponIndicator* createWeaponIndicator(Player* who, Point where);
void drawWeaponIndicator(WeaponIndicator* w);

#endif // WEAPONINDICATOR_H
