#include "bullet.h"
#include "behavior.h"

static void destroyBullet(Bullet* b)
{
    destroy(b->behavior);
}

Bullet* createBullet(Point where)
{
    Animation* anim;

    Bullet* b = new(Bullet);
    b->destroy = destroyBullet;
    b->behavior = createBoring(-3);
    b->position = where;

    anim = createAnimation();
    anim->images = newArray(Image, 1);
    anim->images[0] = makeImage(0x00D97B2, 2, 6);

    b->animation = anim;

    animationBeginning(b);

    return b;
}
