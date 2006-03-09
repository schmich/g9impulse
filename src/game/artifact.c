#include "artifact.h"

static void destroyArtifact(Artifact* a)
{
    destroy(a->behavior);
}

Artifact* createArtifact(Animation* anim,
                         uint8 initFrame,
                         Behavior* behavior,
                         Point where,
                         AffectFn onAffect)
{
    Artifact* a = new(Artifact);
    a->destroy = destroyArtifact;
    a->behavior = behavior;
    a->position = where;
    a->animation = anim;
    a->currentFrame = initFrame;
    a->affect = onAffect;

    return a;
}

void affect(Artifact* a, Player* who, World* world)
{
    a->affect(a, who, world);
}
