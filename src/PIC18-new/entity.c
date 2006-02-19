#include "entity.h"

Image* currentImage(Entity* what)
{
    return &what->animation->images[what->currentFrame];
}

void drawEntity(Entity* what)
{
    drawImage(currentImage(what), what->position, true);
}

uint8 entityWidth(Entity* what)
{
    return currentImage(what)->width;
}

uint8 entityHeight(Entity* what)
{
    return currentImage(what)->height;
}

void animationIncrement(Entity* what)
{
    if (++what->currentFrame > what->animation->numImages)
        animationBeginning(what);
}

void animationDecrement(Entity* what)
{
    if (what->currentFrame == 0)
        animationEnding(what);
    else
        what->currentFrame--;
}

void animationBeginning(Entity* what)
{
    what->currentFrame = 0;
}

void animationEnding(Entity* what)
{
    what->currentFrame = what->animation->numImages - 1;
}
