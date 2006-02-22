#include "background.h"
#include "animation.h"

static void scrollBackground(Updateable* bg, World* world)
{
    currentImage(bg)->address -= currentImage(bg)->width;
}

static void destroyBackground(Background* bg)
{
    destroy(bg->animation);
    destroy(bg->behavior);
}

Background* createBackground(Image image)
{
    Animation* anim = createAnimation();

    Background* bg = new(Background);
    bg->destroy = destroyBackground;
    bg->behavior = createBehavior(scrollBackground);
    bg->position = makePoint(0, 0);

    anim->images = newArray(Image, 1);
    anim->images[0] = image;
    anim->numImages = 1;
    bg->animation = anim;

    animationBeginning(bg);

    return bg;
}
