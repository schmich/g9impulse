#include "explosion.h"
#include "behavior.h"

static Animation theTinyAnimation;
static bool theTinyAnimationInit = false;

static Animation theSmallAnimation;
static bool theSmallAnimationInit = false;

static Animation* tinyAnimation(void)
{
    if (!theTinyAnimationInit)
    {
        theTinyAnimation.numImages = 2;
        theTinyAnimation.images = newArray(Image, theTinyAnimation.numImages);

        theTinyAnimation.images[0] = makeImage(0x00111160, 5, 10);
        theTinyAnimation.images[1] = makeImage(0x00111165, 5, 10);
        
        theTinyAnimationInit = true;
    }

    return &theTinyAnimation;
}

static Animation* smallAnimation(void)
{
    if (!theSmallAnimationInit)
    {
        theSmallAnimation.numImages = 8;
        theSmallAnimation.images = newArray(Image, theSmallAnimation.numImages);

        theSmallAnimation.images[0] = makeImage(0x000DD7C0, 20, 40);
        theSmallAnimation.images[1] = makeImage(0x000DD7D4, 20, 40);
        theSmallAnimation.images[2] = makeImage(0x000DD7E8, 20, 40);
        theSmallAnimation.images[3] = makeImage(0x000DD7FC, 20, 40);
        theSmallAnimation.images[4] = makeImage(0x000DD810, 20, 40);
        theSmallAnimation.images[5] = makeImage(0x000DD824, 20, 40);
        theSmallAnimation.images[6] = makeImage(0x000DD838, 20, 40);
        theSmallAnimation.images[7] = makeImage(0x000DD84C, 20, 40);
        
        theSmallAnimationInit = true;
    }

    return &theSmallAnimation;
}

static void destroyExplosion(Explosion* e)
{
    destroy(e->behavior);
}

Explosion* createExplosion(Point where, uint8 size, uint8 frames)
{
    Explosion* e = new(Explosion);
    e->destroy = destroyExplosion;
    e->behavior = createAnimator(frames);
    e->position = where;

    switch (size)
    {
        case EXPLOSION_TINY:
            e->animation = tinyAnimation();
            break;

        case EXPLOSION_SMALL:
            e->animation = smallAnimation();
            break;
    }

    animationBeginning(e);

    return e;
}
