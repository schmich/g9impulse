#ifndef NUKEMETER_H
#define NUKEMETER_H

#include "common.h"
#include "sprite.h"
#include "player.h"

typedef struct NukeMeter
{
    SPRITE_BASE;
    Player* player;
} NukeMeter;

NukeMeter* createNukeMeter(Player* who, Point where);
void drawNukeMeter(NukeMeter* n);

#endif // NUKEMETER_H
