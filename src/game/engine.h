#ifndef ENGINE_H
#define ENGINE_H

#include "common.h"
#include "updateable.h"

typedef struct Player Player;

typedef struct Engine
{
    UPDATEABLE_BASE;
    Player* player;
    int16   yPrevious;
    uint8   frameDelay;
    Animation* idle;
    Animation* thrust;
    Animation* reverse;
    Animation* boost;
} Engine;

Engine* createEngine(Player* who);

#endif // ENGINE_H
