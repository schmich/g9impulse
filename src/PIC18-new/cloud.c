#include "cloud.h"
#include "behavior.h"

static Animation theTinyAnim;
static Animation theSmallAnim;
static Animation theMediumAnim;
static Animation theLargeAnim;

static bool theTinyAnimInit   = false;
static bool theSmallAnimInit  = false;
static bool theMediumAnimInit = false;
static bool theLargeAnimInit  = false;

static Animation* tinyAnimation(void)
{
    if (!theTinyAnimInit)
    {
        theTinyAnim.numImages = 1;
        theTinyAnim.images = newArray(Image, theTinyAnim.numImages);

        theTinyAnim.images[0] = makeImage(0x00199FBE, 37, 125);

        theTinyAnimInit = true;
    }

    return &theTinyAnim;
}

static Animation* smallAnimation(void)
{
    if (!theSmallAnimInit)
    {
        theSmallAnim.numImages = 1;
        theSmallAnim.images = newArray(Image, theSmallAnim.numImages);

        theSmallAnim.images[0] = makeImage(0x00199BA8, 77, 92);

        theSmallAnimInit = true;
    }

    return &theSmallAnim;
}

static Animation* mediumAnimation(void)
{
    if (!theMediumAnimInit)
    {
        theMediumAnim.numImages = 1;
        theMediumAnim.images = newArray(Image, theMediumAnim.numImages);

        theMediumAnim.images[0] = makeImage(0x0019FCDF, 65, 217);

        theMediumAnimInit = true;
    }

    return &theMediumAnim;
}

static Animation* largeAnimation(void)
{
    if (!theLargeAnimInit)
    {
        theLargeAnim.numImages = 1;
        theLargeAnim.images = newArray(Image, theLargeAnim.numImages);

        theLargeAnim.images[0] = makeImage(0x0019DDEE, 76, 306);

        theLargeAnimInit = true;
    }

    return &theLargeAnim;
}

static void destroyCloud(Cloud* c)
{
    destroy(c->behavior);
}

Cloud* createCloud(int8 x, uint8 recess, uint8 speed, uint8 size)
{
    int16 height;

    Cloud* c = new(Cloud);
    c->destroy = destroyCloud;
    c->behavior = createBoring(speed);

    switch (size)
    {
        case CLOUD_TINY:
            c->animation = tinyAnimation();
            break;

        case CLOUD_SMALL:
            c->animation = smallAnimation();
            break;

        case CLOUD_MEDIUM:
            c->animation = mediumAnimation();
            break;

        case CLOUD_LARGE:
            c->animation = largeAnimation();
            break;
    }

    animationBeginning(c);
    
    height = entityHeight(c);
    c->position.x = x;
    c->position.y = -height - recess;

    return c;
}
