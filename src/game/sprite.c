#include "sprite.h"

rom Image* currentImage(Sprite* what)
{
    return &what->animation->images[what->currentFrame];
}

void drawSprite(Sprite* what)
{
    rom Image* curr = currentImage(what);
    drawImage(curr, what->position, curr->transparent);
}

Sprite* createSprite(Animation* anim, uint8 initFrame, Point where)
{
    Sprite* s = new(Sprite);
    s->destroy = nullDestroy;
    s->animation = anim;
    s->currentFrame = initFrame;
    s->position = where;
    
    return s;
}

uint16 spriteWidth(Sprite* what)
{
    return currentImage(what)->width;
}

uint16 spriteHeight(Sprite* what)
{
    return currentImage(what)->height;
}

int16 spriteLeft(Sprite* what)
{
    return what->position.x;
}

int16 spriteTop(Sprite* what)
{
    return what->position.y;
}

int16 spriteRight(Sprite* what)
{
    return what->position.x + spriteWidth(what);
}

int16 spriteBottom(Sprite* what)
{
    return what->position.y + spriteHeight(what);
}

Point spriteCenter(Sprite* what)
{
    Point p;
    p.x = what->position.x + (spriteWidth(what) / 2);
    p.y = what->position.y + (spriteHeight(what) / 2);
    return p;
}

Point spriteCenterTop(Sprite* what)
{
    Point p;
    p.x = what->position.x + (spriteWidth(what) / 2);
    p.y = spriteTop(what);
    return p;
}

Point spriteCenterBottom(Sprite* what)
{
    Point p;
    p.x = what->position.x + (spriteWidth(what) / 2);
    p.y = spriteBottom(what);
    return p;
}

Point spriteLeftCenter(Sprite* what)
{
    Point p;
    p.x = spriteLeft(what);
    p.y = what->position.y + (spriteHeight(what) / 2);
    return p;
}

Point spriteRightCenter(Sprite* what)
{
    Point p;
    p.x = spriteRight(what);
    p.y = what->position.y + (spriteHeight(what) / 2);
    return p;
}

Point spriteLeftLower(Sprite* what)
{
    Point p;
    p.x = spriteLeft(what);
    p.y = spriteBottom(what);
    return p;
}

Point spriteRightLower(Sprite* what)
{
    Point p;
    p.x = spriteRight(what);
    p.y = spriteBottom(what);
    return p;
}

Point spriteLeftUpper(Sprite* what)
{
    Point p;
    p.x = spriteLeft(what);
    p.y = spriteTop(what);
    return p;
}

Point spriteRightUpper(Sprite* what)
{
    Point p;
    p.x = spriteRight(what);
    p.y = spriteTop(what);
    return p;
}

bool animationNext(Sprite* what)
{
    if (++what->currentFrame >= what->animation->numImages)
    {
        animationBeginning(what);
        return true;
    }

    return false;
}

bool animationPrevious(Sprite* what)
{
    if (what->currentFrame == 0)
    {
        animationEnding(what);
        return true;
    }
    else
    {
        what->currentFrame--;
        return false;
    }
}

void animationBeginning(Sprite* what)
{
    what->currentFrame = 0;
}

void animationEnding(Sprite* what)
{
    what->currentFrame = what->animation->numImages - 1;
}

void setSpriteCenter(Sprite* s, Point p)
{
    s->position.x = p.x - spriteWidth(s) / 2; 
    s->position.y = p.y - spriteHeight(s) / 2; 
}

void setSpriteCenterTop(Sprite* s, Point p)
{
    s->position.x = p.x - spriteWidth(s) / 2; 
    s->position.y = p.y;
}

void setSpriteCenterBottom(Sprite* s, Point p)
{
    s->position.x = p.x - spriteWidth(s) / 2; 
    s->position.y = p.y - spriteHeight(s);
}

void setSpriteLeftCenter(Sprite* s, Point p)
{
    s->position.x = p.x;
    s->position.y = p.y - spriteHeight(s) / 2;
}

void setSpriteRightCenter(Sprite* s, Point p)
{
    s->position.x = p.x - spriteWidth(s);
    s->position.y = p.y - spriteHeight(s) / 2;
}

void setSpriteLeftLower(Sprite* s, Point p)
{
    s->position.x = p.x;
    s->position.y = p.y - spriteHeight(s);
}

void setSpriteRightLower(Sprite* s, Point p)
{
    s->position.x = p.x - spriteWidth(s);
    s->position.y = p.y - spriteHeight(s);
}

void setSpriteLeftUpper(Sprite* s, Point p)
{
    s->position.x = p.x;
    s->position.y = p.y;
}

void setSpriteRightUpper(Sprite* s, Point p)
{
    s->position.x = p.x - spriteWidth(s);
    s->position.y = p.y;
}

Point screenCenter(void)
{
    return makePoint(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

Point screenCenterTop(void)
{
    return makePoint(SCREEN_WIDTH / 2, 0);
}

Point screenCenterBottom(void)
{
    return makePoint(SCREEN_WIDTH / 2, SCREEN_HEIGHT);
}

Point screenLeftCenter(void)
{
    return makePoint(0, SCREEN_HEIGHT / 2);
}

Point screenRightCenter(void)
{
    return makePoint(SCREEN_WIDTH, SCREEN_HEIGHT / 2);
}

Point screenLeftLower(void)
{
    return makePoint(0, SCREEN_HEIGHT);
}

Point screenRightLower(void)
{
    return makePoint(SCREEN_WIDTH, SCREEN_HEIGHT);
}

Point screenLeftUpper(void)
{
    return makePoint(0, 0);
}

Point screenRightUpper(void)
{
    return makePoint(SCREEN_WIDTH, 0);
}
