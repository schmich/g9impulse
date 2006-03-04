#ifndef SPRITE_H
#define SPRITE_H

#include "common.h"
#include "animation.h"
#include "destroyable.h"

#define SPRITE_BASE DESTROYABLE_BASE; \
                    Point      position; \
                    Animation* animation; \
                    uint8      currentFrame

typedef struct Sprite
{
    SPRITE_BASE;
} Sprite;

void drawSprite(Sprite* what);

uint16 spriteWidth(Sprite* what);
uint16 spriteHeight(Sprite* what);
int16 spriteLeft(Sprite* what);
int16 spriteTop(Sprite* what);
int16 spriteRight(Sprite* what);
int16 spriteBottom(Sprite* what);

Point spriteCenter(Sprite* what);
Point spriteCenterTop(Sprite* what);
Point spriteCenterBottom(Sprite* what);
Point spriteLeftCenter(Sprite* what);
Point spriteRightCenter(Sprite* what);
Point spriteLeftLower(Sprite* what);
Point spriteRightLower(Sprite* what);
Point spriteLeftUpper(Sprite* what);
Point spriteRightUpper(Sprite* what);

void alignCenter(Sprite* arrow, Sprite* target);
void alignCenterTop(Sprite* arrow, Sprite* target);
void alignCenterBottom(Sprite* arrow, Sprite* target);
void alignLeftCenter(Sprite* arrow, Sprite* target);
void alignRightCenter(Sprite* arrow, Sprite* target);
void alignLeftLower(Sprite* arrow, Sprite* target);
void alignRightLower(Sprite* arrow, Sprite* target);
void alignLeftUpper(Sprite* arrow, Sprite* target);
void alignRightUpper(Sprite* arrow, Sprite* target);

bool animationNext(Sprite* what);
bool animationPrevious(Sprite* what);
void animationBeginning(Sprite* what);
void animationEnding(Sprite* what);

Image* currentImage(Sprite* what);

#endif // SPRITE_H
