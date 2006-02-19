#ifndef ENTITY_H
#define ENTITY_H

#include "common.h"
#include "gpu_pic.h"
#include "animation.h"

typedef struct Entity
{
    //
    // do not reorder these
    //
    uint8      type;
    Point      position;
    Animation* animation;
    uint8      currentFrame;
} Entity;

void drawEntity(Entity* what);

uint8 entityWidth(Entity* what);
uint8 entityHeight(Entity* what);

void animationIncrement(Entity* what);
void animationDecrement(Entity* what);
void animationBeginning(Entity* what);
void animationEnding(Entity* what);

#endif // ENTITY_H
