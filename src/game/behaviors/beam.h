#ifndef BEAM_H
#define BEAM_H

#include "common.h"
#include "updateable.h"

typedef struct Beam
{
    BEHAVIOR_BASE;
    Player* player;
} Beam;

Beam* createBeam(Player* player);

#endif // BEAM_H
