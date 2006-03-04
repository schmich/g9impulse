#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "killable.h"
#include "projectile.h"

typedef struct Player
{
    KILLABLE_BASE;

    Point       momentum;
    uint8       heat;
    Projectile* weapon;
} Player;

Player* createPlayer(Point where);

void rollLeft(Player* who);
void rollRight(Player* who);
void noRoll(Player* who);

#endif // PLAYER_H
