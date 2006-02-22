#include "background.h"

static void scrollBackground(Updateable* bg)
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
    Background* bg = new(Background);
    bg->destroy = destroyBackground;
    bg->behavior = createBehavior(scrollBackground);
    bg->position = makePoint(0, 0);
    bg->currentFrame = 0;

    Animation* a = createAnimation();
    a->images = newArray(Image, 1);
    a->images[0] = image;
    a->numImages = 1;

    bg->animation = a;
}
