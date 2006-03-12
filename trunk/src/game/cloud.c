#include "cloud.h"
#include "boring.h"
#include "cloud.anim.inl"

static void destroyCloud(Cloud* c)
{
    destroy(c->behavior);
}

Cloud* createCloud(int16 x, int16 recess, int8 speed, uint8 size)
{
    int16 height;

    Cloud* c = new(Cloud);
    c->destroy = destroyCloud;
    c->behavior = createBoring(speed, 1);

    switch (size)
    {
        case CLOUD_TINY:
            c->animation = tinyCloudAnimation();
            break;

        case CLOUD_SMALL:
            c->animation = smallCloudAnimation();
            break;

        case CLOUD_MEDIUM:
            c->animation = mediumCloudAnimation();
            break;

        case CLOUD_LARGE:
            c->animation = largeCloudAnimation();
            break;
            
        case CLOUD_COVER:
            c->animation = cloudCoverAnimation();
            break;
    }

    animationBeginning(c);
    
    height = spriteHeight(c);
    c->position.x = x;
    c->position.y = -height - recess;

    return c;
}
