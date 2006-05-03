#include "beam.h"

static uint8 updateBeam(Updateable* who, World* world)
{
    Beam* b = who->behavior;
    setSpriteCenterBottom(who, spriteCenterTop(b->player));

    return UPDATE_KEEP;
}

Beam* createBeam(Player* player)
{
    Beam* b = new(Beam);
    b->destroy = nullDestroy;
    b->update = updateBeam;
    b->player = player;

    return b;
}
