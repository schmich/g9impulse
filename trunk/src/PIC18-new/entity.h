#ifndef ENTITY_H
#define ENTITY_H

#include "common.h"
#include "animation.h"
#include "destroyable.h"

#define ENTITY_BASE DESTROYABLE_BASE; \
                    Point      position; \
                    Animation* animation; \
                    uint8      currentFrame

typedef struct Entity
{
    ENTITY_BASE;
} Entity;

void drawEntity(Entity* what);

uint16 entityWidth(Entity* what);
uint16 entityHeight(Entity* what);
int16 entityLeft(Entity* what);
int16 entityTop(Entity* what);
int16 entityRight(Entity* what);
int16 entityBottom(Entity* what);

Point entityCenter(Entity* what);
Point entityCenterTop(Entity* what);
Point entityCenterBottom(Entity* what);
Point entityLeftCenter(Entity* what);
Point entityRightCenter(Entity* what);
Point entityLeftLower(Entity* what);
Point entityRightLower(Entity* what);
Point entityLeftUpper(Entity* what);
Point entityRightUpper(Entity* what);

void alignCenter(Entity* arrow, Entity* target);
void alignCenterTop(Entity* arrow, Entity* target);
void alignCenterBottom(Entity* arrow, Entity* target);
void alignLeftCenter(Entity* arrow, Entity* target);
void alignRightCenter(Entity* arrow, Entity* target);
void alignLeftLower(Entity* arrow, Entity* target);
void alignRightLower(Entity* arrow, Entity* target);
void alignLeftUpper(Entity* arrow, Entity* target);
void alignRightUpper(Entity* arrow, Entity* target);

bool animationNext(Entity* what);
bool animationPrevious(Entity* what);
void animationBeginning(Entity* what);
void animationEnding(Entity* what);

Image* currentImage(Entity* what);

#endif // ENTITY_H
