#include "entity.h"

Image* currentImage(Entity* what)
{
    return &what->animation->images[what->currentFrame];
}

void drawEntity(Entity* what)
{
    Image* curr = currentImage(what);
    drawImage(curr, what->position, curr->transparent);
}

uint16 entityWidth(Entity* what)
{
    return currentImage(what)->width;
}

uint16 entityHeight(Entity* what)
{
    return currentImage(what)->height;
}

int16 entityLeft(Entity* what)
{
    return what->position.x;
}

int16 entityTop(Entity* what)
{
    return what->position.y;
}

int16 entityRight(Entity* what)
{
    return what->position.x + entityWidth(what);
}

int16 entityBottom(Entity* what)
{
    return what->position.y + entityHeight(what);
}

Point entityCenter(Entity* what)
{
    Point p;
    p.x = what->position.x + (entityWidth(what) / 2);
    p.y = what->position.y + (entityHeight(what) / 2);
    return p;
}

Point entityCenterTop(Entity* what)
{
    Point p;
    p.x = what->position.x + (entityWidth(what) / 2);
    p.y = entityTop(what);
    return p;
}

Point entityCenterBottom(Entity* what)
{
    Point p;
    p.x = what->position.x + (entityWidth(what) / 2);
    p.y = entityBottom(what);
    return p;
}

Point entityLeftCenter(Entity* what)
{
    Point p;
    p.x = entityLeft(what);
    p.y = what->position.y + (entityHeight(what) / 2);
    return p;
}

Point entityRightCenter(Entity* what)
{
    Point p;
    p.x = entityRight(what);
    p.y = what->position.y + (entityHeight(what) / 2);
    return p;
}

Point entityLeftLower(Entity* what)
{
    Point p;
    p.x = entityLeft(what);
    p.y = entityBottom(what);
    return p;
}

Point entityRightLower(Entity* what)
{
    Point p;
    p.x = entityRight(what);
    p.y = entityBottom(what);
    return p;
}

Point entityLeftUpper(Entity* what)
{
    Point p;
    p.x = entityLeft(what);
    p.y = entityTop(what);
    return p;
}

Point entityRightUpper(Entity* what)
{
    Point p;
    p.x = entityRight(what);
    p.y = entityTop(what);
    return p;
}

bool animationNext(Entity* what)
{
    if (++what->currentFrame >= what->animation->numImages)
    {
        animationBeginning(what);
        return true;
    }

    return false;
}

bool animationPrevious(Entity* what)
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

void animationBeginning(Entity* what)
{
    what->currentFrame = 0;
}

void animationEnding(Entity* what)
{
    what->currentFrame = what->animation->numImages - 1;
}

void alignCenter(Entity* arrow, Entity* target)
{
    Point center = entityCenter(target);
    arrow->position.x = center.x - entityWidth(arrow) / 2;
    arrow->position.y = center.y - entityHeight(arrow) / 2;
}

void alignCenterTop(Entity* arrow, Entity* target)
{
    Point centerTop = entityCenterTop(target);
    arrow->position.x = centerTop.x - entityWidth(arrow) / 2;
    arrow->position.y = centerTop.y;
}

void alignCenterBottom(Entity* arrow, Entity* target)
{
    Point centerBottom = entityCenterBottom(target);
    arrow->position.x = centerBottom.x - entityWidth(arrow) / 2;
    arrow->position.y = centerBottom.y - entityHeight(arrow);
}

void alignLeftCenter(Entity* arrow, Entity* target)
{
    Point leftCenter = entityLeftCenter(target);
    arrow->position.x = leftCenter.x;
    arrow->position.y = leftCenter.y - entityHeight(arrow) / 2;
}

void alignRightCenter(Entity* arrow, Entity* target)
{
    Point rightCenter = entityRightCenter(target);
    arrow->position.x = rightCenter.x - entityWidth(arrow);
    arrow->position.y = rightCenter.y - entityHeight(arrow) / 2;
}

void alignLeftLower(Entity* arrow, Entity* target)
{
    Point leftLower = entityLeftLower(target);
    arrow->position.x = leftLower.x;
    arrow->position.y = leftLower.y - entityHeight(arrow);
}

void alignRightLower(Entity* arrow, Entity* target)
{
    Point rightLower = entityRightLower(target);
    arrow->position.x = rightLower.x - entityWidth(arrow);
    arrow->position.y = rightLower.y - entityHeight(arrow);
}

void alignLeftUpper(Entity* arrow, Entity* target)
{
    Point leftUpper = entityLeftUpper(target);
    arrow->position.x = leftUpper.x;
    arrow->position.y = leftUpper.y;
}

void alignRightUpper(Entity* arrow, Entity* target)
{
    Point rightUpper = entityRightUpper(target);
    arrow->position.x = rightUpper.x - entityWidth(arrow);
    arrow->position.y = rightUpper.y;
}
