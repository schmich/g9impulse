#include "background.h"
#include "animation.h"
#include "background.anim.inl"
#include "gpu.h"

#define LOOP_END    7229
#define LOOP_START  6824

static uint8 updateBackground(Background* bg, World* world)
{
    if (++bg->step == 2)
    {
        ++bg->progress;
        if (++bg->offset == LOOP_END)
            bg->offset = LOOP_START;

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
    bg->progress = bg->offset = SCREEN_HEIGHT;
    bg->step = 0;

    bg->animation = backgroundAnimation();
    animationBeginning(bg);

    return bg;
}

void drawBackground(Background* bg)
{
    rom Image* img = currentImage(bg);
    uint32 offset = (uint32)bg->offset * SCREEN_WIDTH;

    drawFullscreen(img->address - offset);
}
