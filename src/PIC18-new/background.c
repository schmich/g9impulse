#include "background.h"
#include "animation.h"

static void scrollBackground(Background* bg, uint16 lines)
{
    currentImage(bg)->address -= lines * SCREEN_WIDTH;
}

static uint8 updateBackground(Background* bg, World* world)
{
    if (++bg->step == 2)
    {
        scrollBackground(bg, 1);
        bg->progress++;
        bg->step = 0;
    }

    return UPDATE_KEEP;
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
    bg->behavior = createBehavior(updateBackground);
    bg->position = makePoint(0, 0);
    bg->progress = SCREEN_HEIGHT;
    bg->step = 0;

    anim->images = newArray(Image, 1);
    anim->images[0] = image;
    anim->numImages = 1;
    bg->animation = anim;

    animationBeginning(bg);

    scrollBackground(bg, SCREEN_HEIGHT);

    return bg;
}
