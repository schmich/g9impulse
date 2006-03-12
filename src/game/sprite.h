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
Sprite* createSprite(Animation* anim, uint8 initFrame, Point where);

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

Point screenCenter(void);
Point screenCenterTop(void);
Point screenCenterBottom(void);
Point screenLeftCenter(void);
Point screenRightCenter(void);
Point screenLeftLower(void);
Point screenRightLower(void);
Point screenLeftUpper(void);
Point screenRightUpper(void);

void setSpriteCenter(Sprite* s, Point p);
void setSpriteCenterTop(Sprite* s, Point p);
void setSpriteCenterBottom(Sprite* s, Point p);
void setSpriteLeftCenter(Sprite* s, Point p);
void setSpriteRightCenter(Sprite* s, Point p);
void setSpriteLeftLower(Sprite* s, Point p);
void setSpriteRightLower(Sprite* s, Point p);
void setSpriteLeftUpper(Sprite* s, Point p);
void setSpriteRightUpper(Sprite* s, Point p);

bool animationNext(Sprite* what);
bool animationPrevious(Sprite* what);
void animationBeginning(Sprite* what);
void animationEnding(Sprite* what);

rom Image* currentImage(Sprite* what);

#endif // SPRITE_H
