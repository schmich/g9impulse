#include "background.h"
#include "animation.h"
#include "background.anim.inl"

static uint8 updateBackground(Background* bg, World* world)
{
    if (++bg->step == 2)
    {
        bg->progress++;
        bg->step = 0;
    }

    return UPDATE_KEEP;
}

static void destroyBackground(Background* bg)
{
    destroy(bg->behavior);
}

Background* createBackground(void)
{
    Background* bg = new(Background);
    bg->destroy = destroyBackground;
    bg->behavior = createBehavior(updateBackground);
    bg->position = makePoint(0, 0);
    bg->progress = SCREEN_HEIGHT;
    bg->step = 0;

    bg->animation = backgroundAnimation();
    animationBeginning(bg);

    return bg;
}

void drawBackground(Background* bg)
{
    rom Image* img = currentImage(bg);
    uint32 offset = (uint32)bg->progress * SCREEN_WIDTH;

    draw(img->address - offset,
         img->width, img->height,
         0, 0,
         false);
}
