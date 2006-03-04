#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "entity.h"
#include "projectile.h"

typedef struct Player
{
    ENTITY_BASE;

    Point       momentum;
    uint8       heat;
} Player;

Player* createPlayer(Point where);

void rollLeft(Player* who);
void rollRight(Player* who);
void noRoll(Player* who);

#endif // PLAYER_H
