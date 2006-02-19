#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "entity.h"

typedef struct Player
{
    //
    // Entity common base, do not reorder
    //
    uint8      type;
    Point      position;
    Animation* animation;
    uint8      currentFrame;

    //
    // Player specific data
    //
    uint8 health;
    Point momentum;
    uint8 heat;
} Player;

Player* createPlayer(Point where);
void destroyPlayer(Player* who);

void rollLeft(Player* who);
void rollRight(Player* who);
void noRoll(Player* who);

#endif // PLAYER_H
