#ifndef PROXIMITYEXPLOSION_H
#define PROXIMITYEXPLOSION_H

#include "common.h"
#include "updateable.h"
#include "entity.h"

typedef struct ProximityExplosion
{
    BEHAVIOR_BASE;
    Entity* target;
} ProximityExplosion;

ProximityExplosion* createProximityExplosion(Entity* target);

#endif // PROXIMITYEXPLOSION_H
