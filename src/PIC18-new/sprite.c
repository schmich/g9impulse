#include "sprite.h"

Image* currentImage(Sprite* what)
{
    return &what->animation->images[what->currentFrame];
}

void drawSprite(Sprite* what)
{
    Image* curr = currentImage(what);
    drawImage(curr, what->position, curr->transparent);
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

void alignCenter(Sprite* arrow, Sprite* target)
{
    Point center = spriteCenter(target);
    arrow->position.x = center.x - spriteWidth(arrow) / 2;
    arrow->position.y = center.y - spriteHeight(arrow) / 2;
}

void alignCenterTop(Sprite* arrow, Sprite* target)
{
    Point centerTop = spriteCenterTop(target);
    arrow->position.x = centerTop.x - spriteWidth(arrow) / 2;
    arrow->position.y = centerTop.y;
}

void alignCenterBottom(Sprite* arrow, Sprite* target)
{
    Point centerBottom = spriteCenterBottom(target);
    arrow->position.x = centerBottom.x - spriteWidth(arrow) / 2;
    arrow->position.y = centerBottom.y - spriteHeight(arrow);
}

void alignLeftCenter(Sprite* arrow, Sprite* target)
{
    Point leftCenter = spriteLeftCenter(target);
    arrow->position.x = leftCenter.x;
    arrow->position.y = leftCenter.y - spriteHeight(arrow) / 2;
}

void alignRightCenter(Sprite* arrow, Sprite* target)
{
    Point rightCenter = spriteRightCenter(target);
    arrow->position.x = rightCenter.x - spriteWidth(arrow);
    arrow->position.y = rightCenter.y - spriteHeight(arrow) / 2;
}

void alignLeftLower(Sprite* arrow, Sprite* target)
{
    Point leftLower = spriteLeftLower(target);
    arrow->position.x = leftLower.x;
    arrow->position.y = leftLower.y - spriteHeight(arrow);
}

void alignRightLower(Sprite* arrow, Sprite* target)
{
    Point rightLower = spriteRightLower(target);
    arrow->position.x = rightLower.x - spriteWidth(arrow);
    arrow->position.y = rightLower.y - spriteHeight(arrow);
}

void alignLeftUpper(Sprite* arrow, Sprite* target)
{
    Point leftUpper = spriteLeftUpper(target);
    arrow->position.x = leftUpper.x;
    arrow->position.y = leftUpper.y;
}

void alignRightUpper(Sprite* arrow, Sprite* target)
{
    Point rightUpper = spriteRightUpper(target);
    arrow->position.x = rightUpper.x - spriteWidth(arrow);
    arrow->position.y = rightUpper.y;
}
