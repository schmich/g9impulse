#ifndef ENTITY_H
#define ENTITY_H

#include "common.h"
#include "animation.h"
#include "destroyable.h"

#define ENTITY_BASE \
    DESTROYABLE_BASE; \
    Point      position; \
    Animation* animation; \
    uint8      currentFrame;

typedef struct Entity
{
    ENTITY_BASE;
} Entity;

void drawEntity(Entity* what);

uint8 entityWidth(Entity* what);
uint8 entityHeight(Entity* what);
Point entityCenter(Entity* what);

void animationIncrement(Entity* what);
void animationDecrement(Entity* what);
void animationBeginning(Entity* what);
void animationEnding(Entity* what);

#endif // ENTITY_H
