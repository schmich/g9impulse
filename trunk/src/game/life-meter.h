#ifndef LIFEMETER_H
#define LIFEMETER_H

#include "common.h"
#include "sprite.h"
#include "player.h"

typedef struct LifeMeter
{
    SPRITE_BASE;
    Player* player;
} LifeMeter;

LifeMeter* createLifeMeter(Player* who, Point where);
void drawLifeMeter(LifeMeter* n);

#endif // LIFEMETER_H
