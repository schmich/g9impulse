#ifndef ARTIFACT_H
#define ARTIFACT_H

#include "common.h"
#include "updateable.h"
#include "player.h"

struct Artifact;
typedef near rom void (*AffectFn)(struct Artifact*, Player*, World*);

#define ARTIFACT_BASE UPDATEABLE_BASE; \
                      AffectFn affect

typedef struct Artifact
{
    ARTIFACT_BASE;
} Artifact;

Artifact* createArtifact(Animation* anim,
                         uint8 initFrame,
                         Behavior* behavior,
                         Point where,
                         AffectFn onAffect);

void affect(Artifact* a, Player* who, World* world);

#endif // ARTIFACT_H
